// Copyright QuantamOS
#include "kernel.h"

#define MAXCOM 1000
#define MAXLIST 100

void init_shell() {
    clear_screen();
    puts("Welcome to The QuantamOS Shell Cell! get it? yeah no just me okay");
}

int takeInput(char* str) {
    return 0;
}

void printDir() {
    puts("Current Directory: /");
}

void execArgs(char** parsed) {
}

void execArgsPiped(char** parsed, char** parsedpipe) {
}

void openHelp() {
    puts("Help: Use 'cd', 'ls', 'exit', etc.");
}

int ownCmdHandler(char** parsed) {
    if (strcmp(parsed[0], "exit") == 0) {
        puts("Goodbye");
        halt();
    } else if (strcmp(parsed[0], "cd") == 0) {
        return 1;
    } else if (strcmp(parsed[0], "help") == 0) {
        openHelp();
        return 1;
    } else if (strcmp(parsed[0], "hello") == 0) {
        puts("Hello from the kernel shell!");
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
