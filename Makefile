NAME	= minishell

CC		= cc -g
CFLAGS	= -Wall -Wextra -Werror

SRCS	= src/main.c \
		  src/lexer/lexer.c \
		  src/lexer/operator.c \
		  src/parser/parser.c \
		  src/parser/pipes.c \
		  src/parser/redirections.c \
		  src/env/env.c \
		  src/executor/executor.c \
		  src/builtins/cd.c \
		  src/builtins/echo.c \
		  src/builtins/env.c \
		  src/builtins/export.c \
		  src/builtins/pwd.c \
		  src/builtins/unset.c \
		  src/expansion/expand.c \
		  src/res/split.c \
		  src/res/utils.c

OBJS	= $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -lreadline -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
