#include <string.h>
#include "kernel.h"

void execute_command(const char* command) {
    if (strcmp(command, "clear") == 0) {
        clear_screen();
    } else {
        puts("Unknown command\n");
    }
}

char getch() {
    return 'a'
}

void kmain() {
    clear_screen();
    puts("Welcome to Quantaminium Cell!");
    while (true) {
        puts("> ");
        char command[256];
        int i = 0;
        char c;
        while ((c = getch()) != '\n' && i < sizeof(command) - 1) {
            command[i++] = c;
            putchar(c)
        }
        command[i] = '\0';
        execute_command(command);
    }
}