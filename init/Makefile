include /home/$(shell whoami)/Dev/kfs/kfs1/config/config.mk

SRCS_INIT_C := main.c\

SRCS_INIT_S :=

OBJS_INIT := $(addprefix $(OBJS_DIR)/, $(SRCS_INIT_C:.c=.o) $(patsubst %.asm, %.o, $(SRCS_INIT_S)))

$(OBJS_DIR)/%.o: $(INIT_DIR)/%.c
	$(CC) $(CFLAGS) $(CRCFLAGS) -c $(IFLAGS) -o $@ $^

$(OBJS_DIR)/%.o: $(INIT_DIR)/%.asm
	$(AS) $(ASFLAGS) -o $@ $^