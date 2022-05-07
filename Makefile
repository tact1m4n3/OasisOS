# Build docker envirorment: docker build buildenv -t oasis-os
# Run docker envirorment:
# --- Mac OS: docker run --rm -it -v "$(pwd)":/env oasis-os
# --- Windows: docker run --rm -it -v "%cd%":/env oasis-os

SYSROOT = sysroot
IMAGE = image.iso
KERNEL = oasis-kernel

NASM = nasm
CC = gcc
LD = ld

KERNEL_SFLAGS = -f elf64
KERNEL_CFLAGS = -c --freestanding -I kernel/include
KERNEL_LDFLAGS = -n -nostdlib -T kernel/link.ld

KERNEL_OBJS = $(patsubst %.asm, %.o, $(wildcard kernel/*.asm))
KERNEL_OBJS += $(patsubst %.c, %.o, $(wildcard kernel/*.c))
KERNEL_OBJS += $(patsubst %.asm, %.o, $(wildcard kernel/*/*.asm))
KERNEL_OBJS += $(patsubst %.c, %.o, $(wildcard kernel/*/*.c))

all: $(IMAGE)

%.o : %.asm
	mkdir -pv $(dir $@)
	$(NASM) $(KERNEL_SFLAGS) -o $@ $^

%.o : %.c
	mkdir -pv $(dir $@)
	$(CC) $(KERNEL_CFLAGS) -o $@ $^

$(KERNEL): $(KERNEL_OBJS)
	mkdir -pv $(dir $@)
	$(LD) $(KERNEL_LDFLAGS) -o $@ $^

$(IMAGE): $(KERNEL)
	cp $(KERNEL) $(SYSROOT)/boot
	grub-mkrescue -o $@ $(SYSROOT)

.PHONY: clean
clean:
	rm -rf $(IMAGE)
	rm -rf $(KERNEL)
	rm -rf $(KERNEL_OBJS)
