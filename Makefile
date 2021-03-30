NAME = minishell
LIBFT =	Libft/libft.a
CC = gcc
INC = -I includes/
SRCS = ./srcs/main.c ./srcs/parsing/test.c
FLAGS = -Wall -Wextra -Werror
OBJS = $(SRC:.c=.o)
all: $(NAME)
$(NAME): $(OBJS) $(LIBFT)
	@$(CC) $(FLAGS) $(INC) $(SRCS) -o $(NAME) $(LIBFT)
$(LIBFT):
	@make -C ./Libft
clean:
	@rm -f $(OBJS)
	@make clean -C ./libft
fclean: clean
	@rm -f $(NAME)
	@make fclean -C ./libft
re : fclean $(NAME)