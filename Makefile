NAME = minishell

SRC = main.c\
		parsing.c\
		builtins/builtins_utils.c\
		builtins/builtins_utils2.c\
		builtins/env.c\
		builtins/pwd.c\
		builtins/echo.c\
		builtins/export.c\
		builtins/cd.c\
		builtins/unset.c\
		signals/signals.c\
		errors.c\
		lexer/handle_quotes.c\
		lexer/lexer_utils.c\
		lexer/token_reader.c\
		lexer/token_utils.c\
		lexer/parsing_lexar.c\
		execution/pipes.c\
		execution/execution.c\
		execution/free_and_close.c\
		expansion/expansion1.c\
		expansion/expansion_utils.c\

OBJ = $(SRC:.c=.o)

CC = cc

CFLAGS = -Wall -Wextra -Werror #-fsanitize=address
# 033 is a esc in octal ascii, 0 sets all the text attributes to 0 and 32 is green

COLOUR_GREEN=\033[0;32m
COLOUR_END=\033[0m

LIBFT = -L${LIBFTPATH} -lft
LIBFTPATH = libft

# FT_PRINTF = -L${FT_PRINTFPATH} -lft
# FT_PRINTFPATH = ft_printf

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