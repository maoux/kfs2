#files

OBJS :=
HEADERS :=

#includes

include .conf.mk

include $(LIBDIR)/libs.mk

include $(ARCHDIR)/$(ARCH)/files.mk
include $(INITDIR)/files.mk
include $(KERNELDIR)/files.mk

$(shell mkdir -p $(OBJSDIR))


#rules
all: $(NAME)

install:
	sh $(SCRIPTSDIR)/img-create.sh
	sh $(SCRIPTSDIR)/kvm-start.sh

$(NAME): $(OBJS) $(HEADERS)
	@echo "Linking project ..."
	@$(LD) $(LDFLAGS) -o $(NAME) $(OBJS)
	@echo "\033[0;32mBinary $(NAME) successfuly created!\033[0m"

$(OBJSDIR)/%.o: %.c
	@echo "Compiling source file" $^ "to" $@ "..." 
	@$(CC) $(CFLAGS) $(CRCFLAGS) $(DEBUGFLAG) -c $(IFLAGS) -o $@ $^

$(OBJSDIR)/%.o: %.asm
	@echo "Compiling source file" $^ "to" $@ "..." 
	@$(AS) $(ASFLAGS) -o $@ $^

clean:
	@echo "Cleaning" $(OBJSDIR) "..."
	@rm -rf $(OBJS)

fclean:
	@echo "Cleaning" $(OBJSDIR) "..."
	@rm -rf $(OBJS)
	@echo "Cleaning" $(ROOTDIR) "..."
	@rm -f $(NAME)

xclean: fclean
	@echo "Cleaning" $(NAME_IMG) "..."
	@rm -rf $(NAME_IMG)

re: fclean all

.PHONY: clean fclean re all install xclean