BIOS_MBR := ../../mbr.sys
BIOS_BOOT := ../../boot.sys
EFI_BOOT := ../../bootx64.efi

NASM := nasm
CC := x86_64-linux-gnu-gcc
LD := x86_64-linux-gnu-ld
OC := x86_64-linux-gnu-objcopy

COMMON_OBJ_FILES := 

BIOS_OBJ_FILES := bios/boot.o

EFI_INC := -I/usr/include/efi -I/usr/include/efi/x86_64 -I/usr/include/efi/protocol
EFI_LIB := /usr/lib
EFI_LINK := $(EFI_LIB)/elf_x86_64_efi.lds
EFI_CFLAGS := -c $(EFI_INC) -fno-stack-protector -fpic -fshort-wchar \
			-mno-red-zone -Wall -DEFI_FUNCTION_WRAPPER
EFI_LDFLAGS := -nostdlib -znocombreloc -T $(EFI_LINK) -shared \
			-Bsymbolic -L$(EFI_LIB)
EFI_OBJ_FILES := $(EFI_LIB)/crt0-efi-x86_64.o efi/main.o

all: $(BIOS_MBR) $(BIOS_BOOT) $(EFI_BOOT)

bios/%.o: bios/%.s
	mkdir -p $(dir $@)
	$(NASM) -f elf32 -o $@ $^

bios/%.o: bios/%.c
	mkdir -p $(dir $@)
	$(CC) -c -o $@ $^

efi/%.o: efi/%.c
	mkdir -p $(dir $@)
	$(CC) $(EFI_CFLAGS) -o $@ $^

efi64.so: $(COMMON_OBJ_FILES) $(EFI_OBJ_FILES)
	mkdir -p $(dir $@)
	$(LD) $(EFI_LDFLAGS) -o $@ $^ -lefi -lgnuefi

$(BIOS_MBR): bios/mbr.s
	mkdir -p $(dir $@)
	$(NASM) -f bin -o $@ $^

$(BIOS_BOOT): $(COMMON_OBJ_FILES) $(BIOS_OBJ_FILES)
	mkdir -p $(dir $@)
	$(LD) -melf_i386 -T bios/link.ld -o $@ $^

$(EFI_BOOT): efi64.so
	mkdir -p $(dir $@)
	$(OC) -j .text -j .sdata -j .data -j .dynamic \
		-j .dynsym  -j .rel -j .rela -j .reloc \
		--target=efi-app-x86_64 $^ $@
