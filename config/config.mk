PROJECT_NAME	:= kfs1
NAME			?= kfs.bin
NAME_ISO		?= kfs.iso
ARCH			?= x86
MAKE_EXT		?= .mk

#directories

PROJECT_DIR		:=	/home/$(shell whoami)/Dev/kfs/$(PROJECT_NAME)
DEST_DIR		:= $(PROJECT_DIR)/dest
ARCH_DIR		:= $(PROJECT_DIR)/arch
INC_DIR			:= $(PROJECT_DIR)/include
INIT_DIR		:= $(PROJECT_DIR)/init
OBJS_DIR		:= $(PROJECT_DIR)/objs
KERNEL_DIR		:= $(PROJECT_DIR)/kfs
SCRIPTS_DIR		:= $(PROJECT_DIR)/scripts

#compiler / asm
CC			:= /usr/bin/gcc
AS			:= /usr/bin/nasm
LD			:= /usr/bin/ld
LDFILE		:= $(ARCH_DIR)/$(ARCH)/linker.ld

#flags
LDFLAGS		:= -T $(LDFILE) -m elf_i386
ASFLAGS		:= -f elf32
CFLAGS		:= -Wall -Wextra -Werror -m32 -O2
CRCFLAGS	:= -ffreestanding -fno-builtin -nostdlib -nodefaultlibs -fno-stack-protector
IFLAGS		:= -I $(INC_DIR)
DEBUGFLAGS	:= -g
