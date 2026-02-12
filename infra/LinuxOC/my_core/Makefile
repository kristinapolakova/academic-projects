ASM = nasm
CC = gcc
LD = ld
GRUB = grub-mkrescue

ASMFLAGS = -f elf64
CFLAGS = -m64 -ffreestanding -nostdlib -fno-stack-protector -Wall -Wextra -O2
LDFLAGS = -nostdlib -static -T linker.ld -m elf_x86_64

all: mykernel.iso

build/boot.o: boot.asm
	@mkdir -p build
	$(ASM) $(ASMFLAGS) $< -o $@

build/kernel.o: kernel.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

build/kernel.bin: build/boot.o build/kernel.o
	$(LD) $(LDFLAGS) $^ -o $@

mykernel.iso: build/kernel.bin
	@mkdir -p build/isodir/boot/grub
	cp build/kernel.bin build/isodir/boot/
	echo 'set timeout=0' > build/isodir/boot/grub/grub.cfg
	echo 'set default=0' >> build/isodir/boot/grub/grub.cfg
	echo '' >> build/isodir/boot/grub/grub.cfg
	echo 'menuentry "My Kernel" {' >> build/isodir/boot/grub/grub.cfg
	echo '  multiboot /boot/kernel.bin' >> build/isodir/boot/grub/grub.cfg
	echo '  boot' >> build/isodir/boot/grub/grub.cfg
	echo '}' >> build/isodir/boot/grub/grub.cfg
	$(GRUB) -o mykernel.iso build/isodir 2> /dev/null || true

run: mykernel.iso
	qemu-system-x86_64 -cdrom mykernel.iso -m 512M -serial stdio

clean:
	rm -rf build mykernel.iso

.PHONY: all run clean