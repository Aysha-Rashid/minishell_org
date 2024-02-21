NAME = pipex

SRC = main.c \
	child_process.c \
	init.c \
	error_handling.c \
	error_handling1.c \
	path.c \

CC = cc
CFLAGS = -Wall -Wextra -Werror
OBJ = $(SRC:.c=.o)
LIBFT_PATH = libft
LIBFT_LIB = -L${LIBFT_PATH} -lft

$(NAME): $(OBJ)
	@ cd $(LIBFT_PATH) && make all
	@ $(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LIBFT_LIB)

all: $(NAME)

clean:
	rm -rf $(OBJ)
	@ cd $(LIBFT_PATH) && make clean

fclean: clean
	rm -rf $(NAME)
	@ cd $(LIBFT_PATH) && make fclean

re: fclean all