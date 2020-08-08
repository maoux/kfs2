include /home/$(shell whoami)/Dev/kfs/kfs1/config/config.mk

SRCS_LIBC_C = string.c

SRCS_LIBC_A =

OBJS_LIBC	:= $(addprefix $(OBJS_DIR)/, $(SRCS_LIBC_C:.c=.o) $(patsubst %.asm, %.o, $(SRCS_LIBC_A)))


$(OBJS_DIR)/%.o: $(LIBC_DIR)/%.c
	$(CC) $(CFLAGS) $(CRCFLAGS) -c $(IFLAGS) -o $@ $^

$(OBJS_DIR)/%.o: $(LIBC_DIR)/%.asm
	$(AS) $(ASFLAGS) -o $@ $^
