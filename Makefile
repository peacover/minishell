NAME = minishell
LIBFT =	Libft/libft.a
CC = gcc
INC = -I includes/
SRCS =	./srcs/main.c \
		./srcs/parsing/minishell_utils.c \
		./srcs/execution/run_cmdline.c \
		./srcs/execution/builtins/run_builtins.c \
		./srcs/execution/executables/run_cmd.c \
		./srcs/execution/heredoc/heredoc_loop.c \
		./srcs/execution/heredoc/run_heredoc.c \
		./srcs/execution/pipes/parent_process.c \
		./srcs/execution/pipes/pipe_redirect.c \
		./srcs/execution/pipes/run_pipes.c \
		./srcs/execution/redirect/open_files.c \
		./srcs/execution/redirect/redirect.c
LDFLAGS	= "-L/Volumes/Samsung_T5/home_dir/homebrew/opt/readline/lib"
CPPFLAGS = "-I/Volumes/Samsung_T5/home_dir/homebrew/opt/readline/include"
# LDFLAGS	= "-L/Users/$(USER)/.brew/opt/readline/lib"
# CPPFLAGS="-I/Users/$(USER)/.brew/opt/readline/include"
FLAGS = -Wall -Wextra -Werror -g -lreadline $(LDFLAGS) $(CPPFLAGS) #-fsanitize=address
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
