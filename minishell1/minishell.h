#ifndef MINISHELL_H
# define MINISHELL_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "Libft/libft.h"

char    **ft_split(char const *s, char c);
void    ft_cd(char **args);
void    ft_echo(int argc, char **args);
// void    ft_env(char **env);
// void	*ft_realloc(void *ptr, size_t old_size, size_t new_size);
void    ft_pwd();
void    ft_export(int argc,char **argv, char **env);
void    ft_unset(int argc,char **argv, char **env);
void    ft_checkcmd(char *argv,char **env);
void    ft_env();
// void    add_list(node_t *head,node_t *tail,char *argv,int argc);
#endif