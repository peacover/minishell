NAME = minishell
LIBFT =	Libft/libft.a
CC = gcc
INC = -I includes/
SRCS =	./srcs/main.c \
		./srcs/parsing/test.c \
		./srcs/parsing/minishell_utils.c
FLAGS = -Wall -Wextra -Werror -g -lreadline
all: $(NAME)
$(NAME): $(LIBFT)
	@$(CC) $(FLAGS) $(INC) $(SRCS) -o $(NAME) $(LIBFT)
$(LIBFT):
	@make -C ./Libft
clean:
	@rm -f $(OBJS)
	@make clean -C ./libft
fclean: clean
	@rm -f $(NAME)
	@make fclean -C ./libft
	@rm -rf *.dSYM
re : fclean $(NAME)