# Build docker envirorment: docker build buildenv -t oasisos
# Run docker envirorment:
# --- Mac OS: docker run --rm -it -v "$(pwd)":/env oasisos
# --- Windows: docker run --rm -it -v "%cd%":/env oasisos

IMAGE := image.iso

all: image

image: $(IMAGE)

boot:
ifeq ($(shell make -sqC src/boot || echo 1), 1)
	$(MAKE) -C src/boot
endif

$(IMAGE): boot
	./tools/mkiso.sh $@

run_usb:
	qemu-system-x86_64 -usb $(IMAGE)

run_cdrom:
	qemu-system-x86_64 -cdrom $(IMAGE)

run_efi:
	qemu-system-x86_64 -pflash OVMF.fd -cdrom $(IMAGE)

.PHONY: clean
clean:
	rm -f src/boot/*.o
	rm -f src/boot/*.so
	rm -f src/boot/efi/*.o
	rm -f src/boot/bios/*.o
	rm -f mbr.sys
	rm -f boot.sys
	rm -f bootx64.efi
	rm -f efi.img
	rm -f $(IMAGE)
	rm -rf cdrom
