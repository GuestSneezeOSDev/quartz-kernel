#ifndef KERNEL_H
#define KERNEL_H

void kmain();
void putchar(char c);
void puts(const char* str);
void clear_screen();
void move_cursor();
char getch();
void execute_command(const char* command);
void outb(uint16_t port, uint8_t value);

#endif