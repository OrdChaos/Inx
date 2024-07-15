CROSS_COMPILE = x86_64-elf-

AS = nasm
CC = $(CROSS_COMPILE)g++
LD = $(CROSS_COMPILE)ld

SRC_DIR = src
BUILD_DIR = build

CPPSRCS = $(shell find $(SRC_DIR) -name '*.cpp')
ASMSRCS := $(shell find $(SRC_DIR) -name "*.asm")

CPPOBJS = $(CPPSRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
ASMOBJS := $(ASMSRCS:$(SRC_DIR)/%.asm=$(BUILD_DIR)/%.o)

KERNEL = kernel.elf

INCLUDE_DIRS = -Iinclude

ASFLAGS = -f elf64
CFLAGS = -ffreestanding -m64 -fno-exceptions -fno-rtti -std=c++20 -O2 $(INCLUDE_DIRS)
LDFLAGS = -Tlink.x -nostdlib -static -no-pie

all: $(KERNEL)

$(KERNEL): $(CPPOBJS) $(ASMOBJS)
	$(LD) $(LDFLAGS) -o $(BUILD_DIR)/$@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.asm
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean