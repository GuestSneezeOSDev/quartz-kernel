#include "kernel.h"
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define MAXCOM 1000
#define MAXLIST 100

char current_directory[256] = "/";

typedef struct {
    const char *name;
    bool is_dir;
} DirEntry;

DirEntry root_entries[] = {
    {"bin", true},
    {"home", true},
    {"file.txt", false}
};

DirEntry bin_entries[] = {
    {"busybox", false}
};

DirEntry home_entries[] = {
    {"user", true}
};

DirEntry user_entries[] = {
    {"file2.txt", false}
};

DirEntry* get_entries(const char* path) {
    if (strcmp(path, "/") == 0) return root_entries;
    if (strcmp(path, "/bin") == 0) return bin_entries;
    if (strcmp(path, "/home") == 0) return home_entries;
    if (strcmp(path, "/home/user") == 0) return user_entries;
    return NULL;
}

size_t get_entries_count(const char* path) {
    if (strcmp(path, "/") == 0) return sizeof(root_entries) / sizeof(DirEntry);
    if (strcmp(path, "/bin") == 0) return sizeof(bin_entries) / sizeof(DirEntry);
    if (strcmp(path, "/home") == 0) return sizeof(home_entries) / sizeof(DirEntry);
    if (strcmp(path, "/home/user") == 0) return sizeof(user_entries) / sizeof(DirEntry);
    return 0;
}

void init_shell() {
    clear_screen();
    puts("Welcome to The QuantamOS Shell Cell! get it? yeah no just me okay");
}

int takeInput(char* str) {
    int i = 0;
    char c;
    while ((c = getch()) != '\n') {
        if (c == '\b') {
            if (i > 0) {
                putchar('\b');
                i--;
            }
        } else {
            putchar(c);
            str[i++] = c;
        }
    }
    str[i] = '\0';
    return i != 0;
}

void printDir() {
    puts(current_directory);
    putchar('>');
}

void execArgs(char** parsed) {
    if (strcmp(parsed[0], "busybox") == 0) {
        puts("Busybox execution is not yet implemented.");
    } else {
        puts("Command not found");
    }
}

void execArgsPiped(char** parsed, char** parsedpipe) {
    puts("Piped command execution is not yet implemented.");
}

void openHelp() {
    puts("Help: Use 'cd', 'ls', 'exit', 'echo', 'help'.");
}

int ownCmdHandler(char** parsed) {
    if (strcmp(parsed[0], "exit") == 0) {
        puts("Goodbye");
        halt();
    } else if (strcmp(parsed[0], "cd") == 0) {
        if (parsed[1] == NULL) {
            puts("cd: expected argument");
        } else {
            if (strcmp(parsed[1], "/") == 0 || strcmp(parsed[1], "/bin") == 0 || strcmp(parsed[1], "/home") == 0 || strcmp(parsed[1], "/home/user") == 0) {
                strcpy(current_directory, parsed[1]);
            } else {
                puts("cd: no such file or directory");
            }
        }
        return 1;
    } else if (strcmp(parsed[0], "help") == 0) {
        openHelp();
        return 1;
    } else if (strcmp(parsed[0], "echo") == 0) {
        for (int i = 1; parsed[i] != NULL; i++) {
            puts(parsed[i]);
            if (parsed[i + 1] != NULL) {
                putchar(' ');
            }
        }
        putchar('\n');
        return 1;
    } else if (strcmp(parsed[0], "ls") == 0) {
        DirEntry* entries = get_entries(current_directory);
        size_t count = get_entries_count(current_directory);
        if (entries != NULL) {
            for (size_t i = 0; i < count; i++) {
                puts(entries[i].name);
                if (entries[i].is_dir) {
                    puts("/");
                }
                putchar(' ');
            }
            putchar('\n');
        } else {
            puts("ls: cannot access '");
            puts(current_directory);
            puts("': No such file or directory");
        }
        return 1;
    }

    return 0;
}

int parsePipe(char* str, char** strpiped) {
    for (int i = 0; i < 2; i++) {
        strpiped[i] = strsep(&str, "|");
        if (strpiped[i] == NULL)
            break;
    }

    return (strpiped[1] != NULL) ? 1 : 0;
}

void parseSpace(char* str, char** parsed) {
    for (int i = 0; i < MAXLIST; i++) {
        parsed[i] = strsep(&str, " ");
        if (parsed[i] == NULL)
            break;
        if (strlen(parsed[i]) == 0)
            i--;
    }
}

int processString(char* str, char** parsed, char** parsedpipe) {
    char* strpiped[2];
    int piped = parsePipe(str, strpiped);

    if (piped) {
        parseSpace(strpiped[0], parsed);
        parseSpace(strpiped[1], parsedpipe);
    } else {
        parseSpace(str, parsed);
    }

    if (ownCmdHandler(parsed))
        return 0;
    else
        return 1 + piped;
}

void kmain() {
    char inputString[MAXCOM], *parsedArgs[MAXLIST];
    char* parsedArgsPiped[MAXLIST];
    int execFlag = 0;

    init_shell();

    while (1) {
        printDir();
        if (takeInput(inputString))
            continue;
        execFlag = processString(inputString, parsedArgs, parsedArgsPiped);
        if (execFlag == 1)
            execArgs(parsedArgs);
        if (execFlag == 2)
            execArgsPiped(parsedArgs, parsedArgsPiped);
    }
}
