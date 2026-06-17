NAME	= minishell

CC		= cc -g
CFLAGS	= -Wall -Wextra

UNAME	:= $(shell uname)
ifeq ($(UNAME), Darwin)
	CFLAGS	+= -I/opt/homebrew/opt/readline/include
	LDFLAGS	= -L/opt/homebrew/opt/readline/lib -lreadline
else
	LDFLAGS	= -lreadline
endif

SRCS	= src/main.c \
			src/res/ft_itoa.c \
		  src/lexer/lexer.c \
	  src/lexer/operator.c \
		  src/parser/parser.c \
		  src/parser/pipes.c \
		  src/parser/redirections.c \
		  src/env/envUtils.c \
		  src/env/env.c \
		  src/executor/executor.c \
			src/builtins/builtins.c \
			src/builtins/cd.c \
		  src/builtins/echo.c \
		  src/builtins/env.c \
		  src/builtins/export.c \
		  src/builtins/pwd.c \
		  src/builtins/unset.c \
			src/builtins/exit.c \
		  src/expansion/expand.c \
		  src/res/split.c \
		  src/res/utils.c	\
			src/executor/super_exec.c \

OBJS	= $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
