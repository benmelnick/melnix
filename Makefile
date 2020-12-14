KERNEL=kernel.elf
SRC=src/
OBJDIR=build/
ISO=iso/
BUILD_DIRS=$(OBJDIR)boot $(OBJDIR)kernel $(OBJDIR)drivers

.PHONY: $(KERNEL)
.PHONY: clean

AS=nasm
CC=gcc
LD=ld
INC_PATH=-I$(SRC)/include
ASFLAGS=-f elf32
CFLAGS=-m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector  -nostartfiles -nodefaultlibs -c -Wall -Wextra $(INC_PATH) -std=c11 -pedantic # -Werror
LDFLAGS=-T $(SRC)/link.ld -melf_i386

DOTFILES = boot/bootloader.o \
		   drivers/framebuf.o \
		   kernel/main.o

OBJ = $(patsubst %,$(OBJDIR)%,$(DOTFILES))

$(OBJDIR)%.o: $(SRC)%.c
	@echo [CC] $<
	@mkdir -p $(BUILD_DIRS)
	@$(CC) $(CFLAGS)  -o $@ $<

$(OBJDIR)%.o: $(SRC)%.s
	@echo [AS] $<
	@mkdir -p $(BUILD_DIRS)
	@$(AS) $(ASFLAGS) -o $@ $<

$(KERNEL): $(OBJ)
	@echo [INFO] Linking Kernel ELF
	@mkdir -p $(BUILD_DIRS)
	@$(LD) $(LDFLAGS) $(OBJ) -o $(OBJDIR)/kernel.elf

all: $(KERNEL)

iso: $(KERNEL)
	@cp $(OBJDIR)/kernel.elf $(ISO)/kernel.elf
	@echo [INFO] Building ISO image
	@genisoimage -R                             \
				-b boot/grub/stage2_eltorito    \
				-no-emul-boot                   \
				-boot-load-size 4               \
				-A os                           \
				-input-charset utf8             \
				-quiet                          \
				-boot-info-table                \
				-o $(OBJDIR)melnix.iso          \
				iso

run: iso
	@echo [INFO] Starting kernel on Bochs
	@bochs -f bochs/bochsrc.txt -q

clean:
	@echo [INFO] Cleaning build directory and other artifacts
	@rm -rf $(OBJDIR) iso/kernel.elf melnix.iso