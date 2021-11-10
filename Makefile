NAME = minishell
LIBFT =	Libft/libft.a
CC = gcc
INC = -I includes/
SRCS =	./srcs/main.c \
		./srcs/run_cmdline.c \
		./srcs/parsing/minishell_utils.c
LDFLAGS	= "-L/Volumes/Samsung_T5/home_dir/homebrew/opt/readline/lib"
CPPFLAGS = "-I/Volumes/Samsung_T5/home_dir/homebrew/opt/readline/include"
# LDFLAGS	= "-L/opt/homebrew/opt/readline/lib"
# CPPFLAGS = "-I/opt/homebrew/opt/readline/include"

FLAGS = -Wall -Wextra -Werror -g -lreadline $(LDFLAGS) $(CPPFLAGS) -fsanitize=address
# FLAGS = -Wall -Wextra -Werror -g -lreadline -fsanitize=address
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
