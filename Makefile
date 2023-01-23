NAME = minishell

CC = gcc

FLAGS = -Wall -Wextra -Werror

SRC = main.c \
	  	data.c \
	  	list_manip.c \
		list_utils.c \
		dirmanagement.c \

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	@make -C ./Libft
	$(CC) $(FLAGS) -I ./ -lreadline -LLibft -lft $(SRC) -o $@ ./Libft/libft.a

clean:
	rm -f $(OBJ)
	@make clean -C ./Libft

fclean:	clean
	rm -f $(NAME)
	@make fclean -C ./Libft

re:	fclean all

