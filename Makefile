BUILD_MODE ?= debug
OVMF_PATH = /usr/share/ovmf/x64

ARCH_TARGET = arch/target/x86_64-unknown-uefi/$(BUILD_MODE)/arch.efi
KERNEL_TARGET = kernel/target/x86_64-inx/$(BUILD_MODE)/kernel

BOOTX64 = qemu/esp/EFI/BOOT/BOOTX64.efi
KERNEL_ELF = qemu/esp/kernel.elf

all: build copy

build:
	cd arch && cargo build $(if $(filter release,$(BUILD_MODE)),--release) -Zbuild-std
	cd kernel && cargo build $(if $(filter release,$(BUILD_MODE)),--release)

copy:
	mkdir -p qemu/esp/EFI/BOOT
	cp $(ARCH_TARGET) $(BOOTX64)
	cp $(KERNEL_TARGET) $(KERNEL_ELF)

run: all
	qemu-system-x86_64 -m 1024 -drive \
		if=pflash,format=raw,file=$(OVMF_PATH)/OVMF_CODE.fd,readonly=on \
		-drive if=pflash,format=raw,file=$(OVMF_PATH)/OVMF_VARS.fd,readonly=on \
		-drive index=0,file=fat:rw:qemu/esp,format=vvfat \
		-net none

clean:
	rm -rf qemu
	cd arch && cargo clean
	cd kernel && cargo clean