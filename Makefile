NAME = minishell
LIBFT =	Libft/libft.a
CC = gcc
INC = -I includes/
SRCS =	./srcs/main.c \
		./srcs/parsing/builtins_cmd.c \
		./srcs/parsing/errors_arg_msg.c \
		./srcs/parsing/fill_list.c \
		./srcs/parsing/fill_list2.c \
		./srcs/parsing/fill_list_env.c \
		./srcs/parsing/garbage_list.c \
		./srcs/parsing/get_args.c \
		./srcs/parsing/get_args2.c \
		./srcs/parsing/handling_dollar.c \
		./srcs/parsing/handling_dollar2.c \
		./srcs/parsing/init.c \
		./srcs/parsing/minishell_utils.c \
		./srcs/parsing/minishell_utils2.c \
		./srcs/parsing/minishell_utils3.c \
		./srcs/parsing/redirections.c \
		./srcs/parsing/redirections2.c \
		./srcs/parsing/redirections3.c \
		./srcs/execution/run_cmdline.c \
		./srcs/execution/heredoc/run_heredoc.c \
		./srcs/execution/heredoc/heredoc_loop.c \
		./srcs/execution/redirect/redirect.c \
		./srcs/execution/redirect/open_files.c \
		./srcs/execution/pipes/run_pipes.c \
		./srcs/execution/pipes/pipe_redirect.c \
		./srcs/execution/pipes/parent_process.c \
		./srcs/execution/utils.c \
		./srcs/execution/builtins/run_builtins.c \
		./srcs/execution/builtins/cd/cd.c \
		./srcs/execution/builtins/cd/utils.c \
		./srcs/execution/builtins/echo/echo.c \
		./srcs/execution/builtins/envs/utils.c \
		./srcs/execution/builtins/envs/getenv.c \
		./srcs/execution/builtins/envs/setenv.c \
		./srcs/execution/builtins/envs/unset.c \
		./srcs/execution/builtins/envs/env.c \
		./srcs/execution/builtins/envs/export.c \
		./srcs/execution/builtins/envs/putenv.c \
		./srcs/execution/builtins/exit/exit.c \
		./srcs/execution/builtins/exit/utils.c \
		./srcs/execution/builtins/pwd/pwd.c \
		./srcs/execution/execs/run_cmd.c \
		./srcs/execution/signal_handlers.c
# LDFLAGS	= "-L/Volumes/Samsung_T5/home_dir/homebrew/opt/readline/lib"
# CPPFLAGS = "-I/Volumes/Samsung_T5/home_dir/homebrew/opt/readline/include"
# LDFLAGS	= "-L/Users/$(USER)/.brew/opt/readline/lib"
# CPPFLAGS="-I/Users/$(USER)/.brew/opt/readline/include"
LDFLAGS	= "-L/opt/homebrew/opt/readline/lib"
CPPFLAGS = "-I/opt/homebrew/opt/readline/include"
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
