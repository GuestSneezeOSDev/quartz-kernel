#include <stdint.h>

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 35
#define VGA_ENTRY_COLOR(fg, bg) ((fg) | (bg) << 4)
#define VGA_ENTRY_DEFAULT_COLOR VGA_ENTRY_COLOR(15, 0)

static uint16_t* const VGA_BUFFER = (uint16_t*)VGA_ADDRESS;
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

void kernel_entry() {
    clear_screen();
    puts("Welcome to Quantaminium x86_64 Kernel!");
    while (1) {
    }
}

void putchar(char c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else {
        VGA_BUFFER[cursor_y * VGA_WIDTH + cursor_x] = (c | VGA_ENTRY_DEFAULT_COLOR << 8);
        cursor_x++;
    }
    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }
    if (cursor_y >= VGA_HEIGHT) {
        scroll_up();
    }
    move_cursor();
}

void puts(const char* str) {
    while (*str) {
        putchar(*str++);
    }
}

void clear_screen() {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; ++i) {
        VGA_BUFFER[i] = ' ' | (VGA_ENTRY_DEFAULT_COLOR << 8);
    }
    cursor_x = 0;
    cursor_y = 0;
    move_cursor();
}

void scroll_up() {
    for (int y = 1; y < VGA_HEIGHT; ++y) {
        for (int x = 0; x < VGA_WIDTH; ++x) {
            VGA_BUFFER[(y - 1) * VGA_WIDTH + x] = VGA_BUFFER[y * VGA_WIDTH + x];
        }
    }
    for (int x = 0; x < VGA_WIDTH; ++x) {
        VGA_BUFFER[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = ' ' | (VGA_ENTRY_DEFAULT_COLOR << 8);
    }
    cursor_y = VGA_HEIGHT - 1;
    move_cursor();
}

void move_cursor() {
    uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;
    outb(0x3D4, 0x0E);
    outb(0x3D5, pos >> 8);
    outb(0x3D4, 0x0F);
    outb(0x3D5, pos);
}

void outb(uint16_t port, uint8_t value) {
    __asm__("outb %0, %1" : : "a"(value), "Nd"(port));
}
