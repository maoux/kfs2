SRCS_TOOLS_C = printk.c

SRCS_TOOLS_A =

OBJS += $(addprefix $(OBJSDIR)/, $(SRCS_TOOLS_C:.c=.o) $(patsubst %.asm, %.o, $(SRCS_TOOLS_A)))