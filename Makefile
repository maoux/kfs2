NAME:=kfs

OBJS_DIR:=obj

install:

all: $(NAME)

$(NAME):

clean:
	rm -rf $(OBJS)

fclean:
	rm -rf $(OBJS)
	rm -f $(NAME)

re: fclean all

.PHONY: clean fclean re all install