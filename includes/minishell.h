/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <yer-raki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 16:09:15 by yer-raki          #+#    #+#             */
/*   Updated: 2021/11/16 18:50:03 by yer-raki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <termios.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <dirent.h>
#include <errno.h>
#include <sys/fcntl.h>
#include "../Libft/libft.h"

# define HISTORY_MAX_SIZE 500
# define KEY_UP 183
# define KEY_DOWN 184
# define KEY_LEFT  186
# define KEY_RIGHT 185
# define ENTER 10
# define KEY_REMOVE 127
# define KEY_TAB 9
# define CTRL_D 4
# define NODE_INDEX 0
# define REDIR_INDEX 1

typedef struct	s_env
{
    char *val; // all line
    char *value; // after command
    char *key; // command
	
    struct s_env * next;
}				t_env;

typedef struct s_red
{
	char			*r_file;
	char			red_op;
	struct s_red	*next;
}				t_red;

typedef struct  s_sep
{
	char    *path; // whereis echo for example
	char    t_sp; // type of separator ; or |
	int		is_red; // =1 if there is > or < or >>
	int     is_builtin; // = 1 if there is builtin for exapmle echo, pwd, ls...
	int 	is_path_in_arg;
	char    *builtin; // name of command ex: echo, pwd ... 
	char    *upper_builtin; // normalized builtin (UPPER)
	char	*lower_builtin;
	char    **args; // commands without builtin
	char	red_op;
	char	*red_args;
	char	**r_args; // r_args[0] is the command and the others are arguments
	int		p_pip;
	
	char	*s_red; //string of redirection
	
	t_env	*env;
	t_red	*red;
	struct s_sep *next;
}               t_sep;

typedef struct	s_redirect
{
	int		input_fd;
	int		output_fd;
	int		is_input;
	int		is_output;
}				t_redirect;

typedef struct s_garbage
{
	void				*str;
	struct s_garbage	*next;
}				t_garbage;

typedef struct	s_data
{
	t_env		*envl;
	t_garbage	*gg;
	char		**envp;
	int		is_forked;
}				t_data;
typedef	struct s_vars
{
	int		i;
	int		end;
	int		start;
	char    *s1;
	char	*ret;
	int		is_dollar;
	char	*v;
	char	*w;
}				t_vars;

t_data g_data;

int		ft_strlen2(char **w);
void	ft_putstr(char *s);
void	ft_putchar(char c);
void	*ft_realloc(void *ptr, size_t old_size, size_t new_size);
void	ft_putnbr(int n);
char	**ft_split_edited(char const *s);
char	*ft_strcpy(char *dest, char *src);
char	**ft_realloc_2(char **old, size_t old_size, size_t new_size);
void    ft_simplecmd(t_sep *node,char *str);
void	ft_checkcmd1(t_sep *node, char *str, int count_pp);
void    ft_exec(t_sep *node);
char    **fill_paramlist(t_sep *node);
void	handling_builtins(t_sep *node, char *s, int start);
int    run_cmdline(t_sep *node, int pipes_num);
char	*handling_dollar(char *s);
void	signal_handler_parent(int sig);
void	signal_handler_heredoc(int sig);
int		add_to_args(int *end, char *s, int *i, t_sep *node);
void	get_args2(char *s, int *start, int *end);
void    set_exit_code(int value);
int		run_heredoc(t_sep *node);
void	redirect(int *stdin_fd, int *stdout_fd, t_sep *node, int *exit_code);
int	run_pipes(t_sep *node, int pipes_num, int *stdin_fd, int *stdout_fd);
int	run_cmd(t_sep *node, int *stdin_fd, int *stdout_fd);
int	check_error(int condition, void *to_free, char *to_print, int exit_code);
int	run_builtins(t_sep *node, int is_in_pipe);
int	heredoc_loop(int *input_fd, t_sep *node, char *file_name);
int	run_parent_process(pid_t *pids, int *num_cmd, int *exit_status, int pipe_fd[2]);
int	pipe_redirect(t_sep *node, pid_t *pids);
int	open_input(t_redirect *redirect, t_sep *node, int *exit_status);
int	open_output(t_redirect *redirect, char type, t_sep *node, int *exit_status);
int		cd(char **args);
int	echo(char **args);
int	check_valid_dir(char **args, char *cwd);
void	goto_oldpwd(int *exit_status, char *cwd);
void	check_tilde(char **args);
int	ft_setenv(char *key, char *value);
char	*ft_getenv(char *key);
void	pwd(void);
int	is_valid_identifier(char *key);
void	free_pair(char **pair);
int	ft_export(char **args);
int	ft_putenv(char *env_var);
int	unset(char **args);
void	env(void);
int	ft_exit(char **args, int is_in_pipe);
char	*get_last_exit_code(void);
int	is_number(char *s);
void	set_value_for_key(char *key, char *value, int *key_exists);
void	set_new_key(char *key, char *value);
int	check_key_syntax(int condition, char *key);
void	check_fill_path(t_sep *node);
void	handling_builtins(t_sep *node, char *s, int start);
int	handling_errors_arg(char *str);
void	error_msg(char *s);
t_env	*fill_env(char **env);
int	fill_list(char *str);
void	add_garbage(void **str);
void	free_garbage(void);
void	get_args(char *s, int start, t_sep *node);
void	get_args2(char *s, int *start, int *end);
int	add_to_args(int *end, char *s, int *i, t_sep *node);
char	*handling_dollar(char *s);
int	check_dollar(char *s);
void	check_first_cmd_dollar(char **s, int *start);
void	init_node_red(t_red *red);
void	init_data(t_vars *vars);
void	init_t_sep(t_sep *node);
char	*red_redim_s(char *s, int start, int end);
char	*str_upper(char *s);
char	*str_lower(char *s);
int	search_sq(char *s, int start, char type);
char	*remove_char(char *s, int i);
char	*handling_bs_dq(char *s);
void	ft_lstadd_back(t_env **alst, t_env *new);
void	ft_lstadd_front(t_env **alst, t_env *new);
void	add_to_args3(t_sep *node, char *str, int *i);
void	add_to_args4(char *s, int *l, char **str, int start);
void	add_to_args5(t_sep *node, char **str, int j, int *i);
void	add_to_args2(int i, int *j, t_sep *node, char **str);
int	check_red(t_sep *node, char *s);
void	free_t2(char **w);


int		ft_strcmp(char *s1, char *s2);
char	*ft_strcpy(char *dest, char *src);

#endif

//TODO  : single quotes/ double quotes/ backslash/ fill args
//TODO2 : dollar/ path builtin
//TODO3 : redirections
//TODO4 : history

// echo $(( $(getconf ARG_MAX) - $(env | wc -c) ))

// bash-5.1$ echo t >>>t | t>><< | t <> !!!! 
// bash: syntax error near unexpected token `>'

// bash-5.1$ >
// bash: syntax error near unexpected token `newline'

// export MallocStackLogging=1


// << dsf sdf > sdfsdf>>fsdf sdf | sdf > sdf | echo tt