# # #==============# # #
# # ### MAKEFILE ### # #
# # #==============# # #

## === VARIABLES === ##

NAME = microshell

SRCS = main.c \
	microshell.c \
	cd.c \
	free.c \
	utils.c

CC = gcc

CFLAGS = -Wall -Wextra -Werror

## === RULES === ##

all: $(NAME)

$(NAME):
	$(CC) -o $(NAME) $(CFLAGS) $(SRCS)

re: fclean all

clean:
	@rm -rf $(OBJS)

fclean: clean
	@rm -rf $(NAME)
