NAME = minishell
LIBFT =	Libft/libft.a
CC = gcc
INC = -I
SRCS =	builtins/main.c \
		builtins/checkcmd.c builtins/ft_env.c\
		Libft/ft_memcpy.c 
FLAGS = #-Wall -Wextra -Werror -g -fsanitize=address


all: $(NAME)
$(NAME):
	@make -C ./Libft
	@cp Libft/libft.a .
	@$(CC) $(FLAGS) $(SRCS) $(INC) $(LIBFT) -o $(NAME)

clean:
	@make clean -C ./libft
fclean: clean
	@rm -f $(NAME)
	@make fclean -C ./libft
re : fclean $(NAME)