CROSS_COMPILE = "x86_64-elf-"
CC = "$(CROSS_COMPILE)gcc"
LD = "$(CROSS_COMPILE)ld"
AS = "nasm"

CFLAGS = "-ffreestanding -O2 -Wall -Wextra"
LDFLAGS = "-T src/kernel/linker.ld"

build:
    @echo "Building kernel..."
    $(AS) -f elf64 src/boot/boot.asm -o boot.o
    $(CC) $(CFLAGS) -c src/kernel/kernel.c -o kernel.o
    $(LD) $(LDFLAGS) -o kernel.bin boot.o kernel.o
    @echo "Build complete."

clean:
    @echo "Cleaning up..."
    rm -f *.o kernel.bin
    @echo "Cleanup complete."

menuconfig:
    @echo "Running menuconfig for BusyBox..."
    cd /tmp/busybox/busybox-${BUSYBOX_VERSION} && make menuconfig

busybox:
    @echo "Building BusyBox..."
    cd /tmp/busybox/busybox-${BUSYBOX_VERSION} && make

install: busybox
    @echo "Installing BusyBox..."
    cd /tmp/busybox/busybox-${BUSYBOX_VERSION} && make CONFIG_PREFIX=$(INSTALL_DIR) install

initramfs:
    @echo "Creating initramfs..."
    @mkdir -p $(INSTALL_DIR)/initramfs/bin
    @cp /tmp/busybox/busybox-${BUSYBOX_VERSION}/busybox $(INSTALL_DIR)/initramfs/bin
    @cd $(INSTALL_DIR)/initramfs && find . | cpio -H newc -o | gzip > ../initramfs.cpio.gz
    @echo "initramfs created."

all: build busybox initramfs

.PHONY: build clean menuconfig busybox install initramfs all
