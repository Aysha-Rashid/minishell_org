NAME = minishell

SRC = main.c\
		parsing.c\
		errors.c\
		utils.c\
		get_next_line.c\
		get_next_line_utils.c\
		builtins/builtins_utils.c\
		builtins/builtins_utils2.c\
		builtins/env.c\
		builtins/pwd.c\
		builtins/echo.c\
		builtins/export.c\
		builtins/cd.c\
		builtins/unset.c\
		signals/signals.c\
		execution/pipes.c\
		execution/execution.c\
		execution/free_and_close.c\
		execution/redirection.c\
		execution/execution_utils.c\
		execution/execution_utils2.c\
		execution/parsing_utils.c\
		execution/heredoc.c\
		expansion/expansion1.c\
		expansion/expansion_utils.c\
		expansion/expansion_utils2.c\

OBJ = $(SRC:.c=.o)

CC = cc

CFLAGS = -Wall -Wextra -Werror -g3
COLOUR_GREEN=\033[0;32m
COLOUR_END=\033[0m

LIBFT = -L${LIBFTPATH} -lft
LIBFTPATH = libft

$(NAME): $(OBJ)
	@ cd $(LIBFTPATH) && make all
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) -lreadline -L/usr/local/opt/readline/lib -I/usr/local/opt/readline/include $(LIBFT)
	@ echo "$(COLOUR_GREEN)compiled $(words $(OBJ)) files $(COLOUR_END)"

all: $(NAME)

clean:
	@ rm -rf $(OBJ)
	@ make fclean -C $(LIBFTPATH)
	@ echo "$(COLOUR_GREEN)Cleaned all 'o' files successfully$(COLOUR_END)"

fclean: clean
	@ rm -rf $(NAME)
	@ echo "$(COLOUR_GREEN)Cleaned Program name successfully$(COLOUR_END)"


re: fclean $(NAME)