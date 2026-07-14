NAME	= minishell

CC		= cc -g
CFLAGS	= -Wall -Wextra -Werror

SRCS	= src/main.c \
				src/signals.c \
				src/douane.c \
				src/cleaner.c \
			src/res/ft_itoa.c \
		  src/lexer/lexer.c \
	  src/lexer/operator.c \
		  src/parser/parser.c \
		  src/parser/parser_utils.c \
		  src/parser/pipes.c \
		  src/parser/redirections.c \
		  src/env/envUtils.c \
		  src/env/env.c \
		  src/env/env_init.c \
		  src/executor/executor.c \
		  src/executor/exec_utils.c \
			src/builtins/builtins.c \
			src/builtins/cd.c \
		  src/builtins/echo.c \
		  src/builtins/env.c \
		  src/builtins/export.c \
		  src/builtins/export_sort.c \
		  src/builtins/export_array.c \
		  src/builtins/export_validate.c \
		  src/builtins/pwd.c \
		  src/builtins/unset.c \
			src/builtins/exit.c \
		  src/expansion/expand.c \
		  src/res/split.c \
		  src/res/utils.c	\
		  src/res/strings.c \
			src/executor/super_cmd.c \
			src/executor/base_cmd.c \
			src/executor/super_exec.c \
			src/executor/heredoc.c \
			src/lexer/lexer_util.c \

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
