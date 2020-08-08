PROJECT_NAME	:= kfs1
NAME			?= kfs.bin
NAME_ISO		?= kfs.iso
NAME_IMG		?= kfs.img
ARCH			?= x86
MAKE_EXT		?= .mk

#directories

DESTDIR			?= dest
ARCHDIR			:= arch
INCDIR			:= include
INITDIR			:= init
LIBDIR			:= lib
OBJSDIR			:= objs
KERNELDIR		:= kfs
SCRIPTSDIR		:= scripts
ROOTDIR			:= $(realpath .)

VPATH			:=	$(INCDIR) $(INCDIR)/$(KERNELDIR)\
					$(KERNELDIR) $(ARCHDIR)/$(ARCH)\
					$(INITDIR) $(LIBDIR)

#compiler / asm
CC			:= /usr/bin/gcc
AS			:= /usr/bin/nasm
LD			:= /usr/bin/ld
LDFILE		:= $(ARCHDIR)/$(ARCH)/linker.ld

#flags
LDFLAGS		:= -T $(LDFILE) -m elf_i386
ASFLAGS		:= -f elf32
CFLAGS		:= -Wall -Wextra -Werror -m32 -O2
CRCFLAGS	:= -ffreestanding -fno-builtin -nostdlib -nodefaultlibs -fno-stack-protector
IFLAGS		:= -I $(INCDIR) -I $(INCDIR)/$(KERNELDIR)
DEBUGFLAGS	:= -g
