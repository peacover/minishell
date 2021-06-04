/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <yer-raki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 16:09:15 by yer-raki          #+#    #+#             */
/*   Updated: 2021/06/04 14:07:00 by yer-raki         ###   ########.fr       */
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
// #include <termcap.h>
#include "../Libft/libft.h"

typedef struct	s_env
{
    char *val; // all line
    char *value; // after command
    char *key; // command
    struct s_env * next;
}				t_env;

typedef struct  s_red
{
	char    type; //input = i < | output = o > | append = a >>
	char    *file; // path file
	char	**args;// args[0] is the command!!! 
	struct s_red   *next;
}               t_red;

typedef struct  s_sep
{
	char    *path; // whereis echo for example
	char    t_sp; // type of separator ; or |
	int		is_red; // =1 if there is > or < or >>
	int     is_builtin; // = 1 if there is builtin for exapmle echo, pwd, ls...
	char    *builtin; // name of command ex: echo, pwd ... 
	char    *upper_builtin; // normalized builtin (UPPER)
	char	*lower_builtin;
	char    **args; // commands without builtin
	t_red   *red; // redirection > or >> or <
	t_env	*env;
	struct s_sep *next;
}               t_sep;

t_env	*g_env;

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