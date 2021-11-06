/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <yer-raki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 16:09:15 by yer-raki          #+#    #+#             */
/*   Updated: 2021/11/06 09:34:26 by yer-raki         ###   ########.fr       */
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
	char	*r_file;
	char	red_op;
	char	*red_args;
	char	**r_args; // r_args[0] is the command and the others are arguments
	int		p_pip;
	
	char	*s_red; //string of redirection
	
	t_env	*env;
	t_red	*red;
	struct s_sep *next;
}               t_sep;

t_env	*g_env;
char **g_envp;

int		ft_strcmp(char *s1, char *s2);
int		ft_strlen2(char **w);
void	ft_putstr(char *s);
void	ft_putchar(char c);
void	*ft_realloc(void *ptr, size_t old_size, size_t new_size);
void	ft_putnbr(int n);
char	**ft_split_edited(char const *s);
void	free_t2(char **w);
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

#endif

//TODO  : single quotes/ double quotes/ backslash/ fill args
//TODO2 : dollar/ path builtin
//TODO3 : redirections
//TODO4 : history

// echo $(( $(getconf ARG_MAX) - $(env | wc -c) ))

// bash-5.1$ echo t >>>t | t>><< | t<> !!!! 
// bash: syntax error near unexpected token `>'

// bash-5.1$ >
// bash: syntax error near unexpected token `newline'

// export MallocStackLogging=1


// << dsf sdf > sdfsdf>>fsdf sdf | sdf > sdf | echo tt