include /home/$(shell whoami)/Dev/kfs/kfs1/config/config.mk

SRCS_ARCH_C :=

SRCS_ARCH_S := boot.asm\

OBJS_ARCH := $(addprefix $(OBJS_DIR)/, $(SRCS_ARCH_C:.c=.o) $(patsubst %.asm, %.o, $(SRCS_ARCH_S)))

$(OBJS_DIR)/%.o: $(ARCH_DIR)/$(ARCH)/%.c
	$(CC) $(CFLAGS) $(CRCFLAGS) -c $(IFLAGS) -o $@ $^

$(OBJS_DIR)/%.o: $(ARCH_DIR)/$(ARCH)/%.asm
	$(AS) $(ASFLAGS) -o $@ $^
