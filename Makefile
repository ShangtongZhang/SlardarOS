NASM := nasm
CC_COMPILER_PATH := /usr/local/gcc-4.8.1-for-linux32/i586-pc-linux/bin
CC_PREFIX := $(CC_COMPILER_PATH)/
CC := $(CC_PREFIX)gcc
LD := $(CC_PREFIX)ld
OBJCOPY := $(CC_PREFIX)objcopy
OBJDUMP := $(CC_PREFIX)objdump
HD := hdiutil
DD := dd
CP := cp
RM := rm
QEMU := qemu-system-i386
V := @

C_FLAGS := -fno-builtin -Wall -ggdb -gstabs -nostdinc -fno-stack-protector -O0 -nostdinc

BUILD_DIR := generated
BOOT_DIR := boot
BOOT_SECTOR_BIN := bootSector.bin
BOOT_LOADER_BIN := loader.bin
OS_IMAGE := SlardarOS.img
EMPTY_IMAGE := empty.img
MOUNT_DIR := /Volumes/SlardarOS/
KERNEL_BIN := kernel.bin

.PHONY: qemu debug os bootSector bootLoader osImage clean kernel

debug: osImage
	$(V) ttab -w eval 'gdb -x utils/gdb.cmd'
	$(V) $(QEMU) -s -S -fda $(BUILD_DIR)/$(OS_IMAGE)
qemu: osImage
	$(V) $(QEMU) -fda $(BUILD_DIR)/$(OS_IMAGE)

os: osImage

bootSector:
	$(V) $(NASM) -I $(BOOT_DIR)/ $(BOOT_DIR)/bootSector.asm -o $(BUILD_DIR)/$(BOOT_SECTOR_BIN)

bootLoader:
	$(V) $(CC) -m32 $(C_FLAGS) -B$(CC_COMPILER_PATH) -c $(BOOT_DIR)/bootLoader.S -o $(BUILD_DIR)/bootLoader.S.o
	$(V) $(CC) -m32 $(C_FLAGS) -B$(CC_COMPILER_PATH) -c $(BOOT_DIR)/bootLoader.c -o $(BUILD_DIR)/bootLoader.c.o
	$(V) $(LD) -m elf_i386 -nostdlib -N -e start -Ttext 0x9000 -o $(BUILD_DIR)/bootLoader.o $(BUILD_DIR)/bootLoader.S.o $(BUILD_DIR)/bootLoader.c.o
	$(V) $(OBJCOPY) -S -O binary $(BUILD_DIR)/bootLoader.o $(BUILD_DIR)/$(BOOT_LOADER_BIN)
	$(V) $(OBJDUMP) -S -D $(BUILD_DIR)/bootLoader.o > $(BUILD_DIR)/bootLoader.dump

kernel:
	#$(V) echo 'hello world!' > $(BUILD_DIR)/$(KERNEL_BIN)

osImage: bootSector bootLoader kernel
	$(V) $(DD) if=/dev/zero of=$(BUILD_DIR)/$(EMPTY_IMAGE) bs=512 count=2880
	$(V) $(DD) if=$(BUILD_DIR)/$(BOOT_SECTOR_BIN) of=$(BUILD_DIR)/$(OS_IMAGE) bs=512 count=1
	$(V) $(DD) if=$(BUILD_DIR)/$(EMPTY_IMAGE) of=$(BUILD_DIR)/$(OS_IMAGE) skip=1 seek=1 bs=512 count=2879
	$(V) $(HD) attach $(BUILD_DIR)/$(OS_IMAGE)
	$(V) $(CP) $(BUILD_DIR)/$(BOOT_LOADER_BIN) $(MOUNT_DIR)/$(BOOT_LOADER_BIN)
	$(V) $(CP) $(BUILD_DIR)/$(KERNEL_BIN) $(MOUNT_DIR)/$(KERNEL_BIN)
	$(V) $(HD) detach $(MOUNT_DIR)

clean:
	$(V) $(RM) $(BUILD_DIR)/*.o
	$(V) $(RM) $(BUILD_DIR)/*.bin
	$(V) $(RM) $(BUILD_DIR)/*.dump
	$(V) $(RM) $(BUILD_DIR)/*.img