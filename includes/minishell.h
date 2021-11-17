/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <yer-raki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 16:09:15 by yer-raki          #+#    #+#             */
/*   Updated: 2021/11/17 18:15:36 by yer-raki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <string.h>
# include <fcntl.h>
# include <signal.h>
# include <termios.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <dirent.h>
# include <errno.h>
# include <sys/fcntl.h>
# include "../Libft/libft.h"

typedef struct s_env
{
	char			*val;
	char			*value;
	char			*key;
	struct s_env	*next;
}					t_env;

typedef struct s_red
{
	char			*r_file;
	char			red_op;
	struct s_red	*next;
}					t_red;

typedef struct s_sep
{
	char			*path;
	char			t_sp;
	int				is_red;
	int				is_builtin;
	int				is_path_in_arg;
	char			*builtin;
	char			*upper_builtin;
	char			*lower_builtin;
	char			**args;
	char			red_op;
	char			*red_args;
	char			**r_args;
	int				p_pip;
	char			*s_red;
	t_env			*env;
	t_red			*red;
	struct s_sep	*next;
}					t_sep;

typedef struct s_redirect
{
	int	input_fd;
	int	output_fd;
	int	is_input;
	int	is_output;
}				t_redirect;

typedef struct s_garbage
{
	void				*str;
	struct s_garbage	*next;
}				t_garbage;

typedef struct s_data
{
	t_env		*envl;
	t_garbage	*gg;
	char		**envp;
	int			is_forked;
}				t_data;

typedef struct s_vars
{
	int		i;
	int		end;
	int		start;
	char	*s1;
	char	*ret;
	int		is_dollar;
	char	*v;
	char	*w;
}				t_vars;

t_data	g_data;

int		ft_strlen2(char **w);
void	ft_putstr(char *s);
void	ft_putchar(char c);
void	*ft_realloc(void *ptr, size_t old_size, size_t new_size);
char	**ft_split_edited(char const *s);
char	*ft_strcpy(char *dest, char *src);
char	**ft_realloc_2(char **old, size_t old_size, size_t new_size);
void	ft_simplecmd(t_sep *node, char *str);
void	ft_checkcmd1(t_sep *node, char *str, int count_pp);
void	ft_exec(t_sep *node);
char	**fill_paramlist(t_sep *node);
void	handling_builtins(t_sep *node, char *s, int start);
int		run_cmdline(t_sep *node, int pipes_num);
char	*handling_dollar(char *s);
void	signal_handler_parent(int sig);
void	signal_handler_heredoc(int sig);
void	get_args2(char *s, int *start, int *end);
void	set_exit_code(int value);
int		run_heredoc(t_sep *node);
void	redirect(int *stdin_fd, int *stdout_fd, t_sep *node, int *exit_code);
int		run_pipes(t_sep *node, int pipes_num, int *stdin_fd, int *stdout_fd);
int		run_cmd(t_sep *node, int *stdin_fd, int *stdout_fd);
int		check_error(int condition, void *to_free,
			char *to_print, int exit_code);
int		run_builtins(t_sep *node, int is_in_pipe);
int		heredoc_loop(int *input_fd, t_sep *node, char *file_name);
int		run_parent_process(pid_t *pids, int *num_cmd,
			int *exit_status, int pipe_fd[2]);
int		pipe_redirect(t_sep *node, pid_t *pids);
int		open_input(t_redirect *redirect, t_sep *node, int *exit_status);
int		open_output(t_redirect *redirect, char type,
			t_sep *node, int *exit_status);
int		cd(char **args);
int		echo(char **args);
int		check_valid_dir(char **args, char *cwd);
void	goto_oldpwd(int *exit_status, char *cwd);
void	check_tilde(char **args);
int		ft_setenv(char *key, char *value);
char	*ft_getenv(char *key);
void	pwd(void);
int		is_valid_identifier(char *key);
void	free_pair(char **pair);
int		ft_export(char **args);
int		ft_putenv(char *env_var);
int		unset(char **args);
void	env(void);
int		ft_exit(char **args, int is_in_pipe);
char	*get_last_exit_code(void);
int		is_number(char *s);
void	set_value_for_key(char *key, char *value, int *key_exists);
void	set_new_key(char *key, char *value);
int		check_key_syntax(int condition, char *key);
void	check_fill_path(t_sep *node);
void	handling_builtins(t_sep *node, char *s, int start);
int		handling_errors_arg(char *str);
void	error_msg(char *s);
t_env	*fill_env(char **env);
void	fill_list(char *str);
void	add_garbage(void **str);
void	free_garbage(void);
void	get_args(char *s, int start, t_sep *node);
void	get_args2(char *s, int *start, int *end);
int		add_to_args(int *end, char *s, int *i, t_sep *node);
int		check_dollar(char *s);
void	check_first_cmd_dollar(char **s, int *start);
void	init_node_red(t_red *red);
void	init_data(t_vars *vars);
void	init_t_sep(t_sep *node);
char	*red_redim_s(char *s, int start, int end);
char	*str_upper(char *s);
char	*str_lower(char *s);
int		search_sq(char *s, int start, char type);
char	*remove_char(char *s, int i);
void	ft_lstadd_back(t_env **alst, t_env *new);
void	ft_lstadd_front(t_env **alst, t_env *new);
void	add_to_args3(t_sep *node, char *str, int *i);
void	add_to_args4(char *s, int *l, char **str, int start);
void	add_to_args5(t_sep *node, char **str, int j, int *i);
void	add_to_args2(int i, int *j, t_sep *node, char **str);
int		check_red(t_sep *node, char *s);
void	free_t2(char **w);
int		check_red(t_sep *node, char *s);
void	red_get_type_file(t_sep *node, char *s);
char	*red_get_file(t_sep *node, char *s, int start, char type);
char	red_get_type(char *s, int start);
void	red_get_cmd_args(t_sep *node);
void	red_add_to_args(int start, int end, int *i, t_sep *node);
void	get_builtin(char *s, t_sep *node);
char	*str_export_split(char *s, int start, int is_dollar);
int		equal_export(char *s, int i);
void	handling_dollar7(char *s, t_vars *vars);
void	check_first_cmd_dollar(char **s, int *start);
void	red_get_cmd_args2(t_sep *node, int *start, int *end, int *i);
int		ft_strcmp(char *s1, char *s2);
char	*ft_strcpy(char *dest, char *src);
#endif
