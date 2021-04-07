/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <yer-raki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 16:09:15 by yer-raki          #+#    #+#             */
/*   Updated: 2021/04/07 16:44:55 by yer-raki         ###   ########.fr       */
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

// typedef struct  s_red
// {
//     char    type; //input = i | output = o | append = a
//     char    *p_file; // path file
//     struct t_red   *next;
// }               t_red;

typedef struct  s_infos
{
    char    **args;
    // char    *path; // whereis echo for example
    // char    t_sp; // type of separator ; or | 
    // char    *cmd; // name of command ex: echo, ls, pwd ... 
    // char    *env; // result of $home for example
    // int     is_env;// = 1 if it is $ 
    // t_red   *red; // redirection | or ; 
    // struct t_infos *next;
}               t_infos;

int		ft_strcmp(char *s1, char *s2);
int		ft_strlen2(char **w);
void	ft_putstr(char *s);
void	ft_putchar(char c);
void	*ft_realloc(void *ptr, size_t old_size, size_t new_size);
void	ft_putnbr(int n);
char	**ft_split_edited(char const *s);
void	free_t2(char **w);
#endif