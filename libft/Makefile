NAME = libft.a

SRC = ft_atoi.c\
	  ft_isalnum.c\
	  ft_isascii.c\
	  ft_isdigit.c\
	  ft_isprint.c\
	  ft_strchr.c\
	  ft_strlen.c\
	  ft_strncmp.c\
	  ft_strrchr.c\
	  ft_tolower.c\
	  ft_toupper.c\
	  ft_memchr.c\
	  ft_strlcpy.c\
	  ft_strlcat.c\
	  ft_strnstr.c\
	  ft_isalpha.c\
	  ft_memset.c\
	  ft_bzero.c\
	  ft_memcpy.c\
	  ft_memmove.c\
	  ft_memcmp.c\
	  ft_calloc.c\
	  ft_strdup.c\
	  ft_substr.c\
	  ft_putchar_fd.c\
	  ft_putstr_fd.c\
	  ft_putnbr_fd.c\
	  ft_itoa.c\
	  ft_putendl_fd.c\
	  ft_strjoin.c\
	  ft_strtrim.c\
	  ft_split.c\
	  ft_strmapi.c\
	  ft_striteri.c\

BONUS = ft_lstnew.c\
		ft_lstadd_front.c\
		ft_lstlast.c\
		ft_lstadd_back.c\
		ft_lstsize.c\
		
CFLAGS = -Wall -Wextra -Werror

CC = cc

OBJ = $(SRC:.c=.o)

BONUSOBJ = $(BONUS:.c=.o)

$(NAME): $(OBJ)
	@ ar -rcs $(NAME) $(OBJ)

bonus: $(BONUSOBJ) 
	@ ar -rcs $(NAME) $(BONUSOBJ)

all: $(NAME)

clean:
	@ rm -f $(OBJ) $(BONUSOBJ)

fclean: clean
	@ rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re