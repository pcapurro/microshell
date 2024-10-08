# # #==============# # #
# # ### MAKEFILE ### # #
# # #==============# # #

## === VARIABLES === ##

NAME = microshell

SRCS = srcs/main.c \
	srcs/microshell.c \
	srcs/cd.c \
	srcs/free.c \
	srcs/utils.c

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
