CROSS_COMPILE = x86_64-elf-
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
AS = nasm

CFLAGS = -ffreestanding -O2 -Wall -Wextra
LDFLAGS = -T src/kernel/linker.ld

all: kernel.bin

kernel.bin: boot.o kernel.o
	$(LD) $(LDFLAGS) -o $@ boot.o kernel.o

boot.o: src/boot/boot.asm
	$(AS) -f elf64 $< -o $@

kernel.o: src/kernel/kernel.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o kernel.bin

.PHONY: all clean
