NAME		:= kfs

#directories
ARCH_DIR	:= arch
INC_DIR		:= include
INIT_DIR	:= init
OBJS_DIR	:= obj
KERNEL_DIR	:= kfs

ifndef KERNEL_SRC
	KERNEL_SRC	:=kfs
endif

#compiler / asm
CC			:= /usr/bin/gcc
AS			:= /usr/bin/nasm

#flags
CFLAGS		:= -Wall -Wextra -Werror
CRCFLAGS	:= -fno-builtin -nostdlib -nodefaultlibs -fno-stack-protector
IFLAGS		:= -I $(INC_DIR)

#files

#includes

include($(KERNEL_SRC)/Makefile)

#debug

#rules
install:

all: $(NAME)

$(NAME):

$(OBJS_DIR)/%.o: %.c
	$(CC) $(CFLAGS) $(CRCFLAGS) -c $(IFLAGS) -o $@ $^

clean:
	rm -rf $(OBJS)

fclean:
	rm -rf $(OBJS)
	rm -f $(NAME)

re: fclean all

.PHONY: clean fclean re all install