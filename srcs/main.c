/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <yer-raki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 16:08:10 by yer-raki          #+#    #+#             */
/*   Updated: 2021/04/07 17:07:01 by yer-raki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// void    handling_errors_args(char *arg, t_infos *infos)
// {
    
// }
void    error_msg(char *s)
{
    ft_putstr(s);
    ft_putchar('\n');
}
int    handling_errors_arg(char *arg)
{
    int i;

    i = 0;
    while (arg[i])
    {
        if (arg[i] == ';' && arg[i + 1] == ';')
            error_msg("syntax error near unexpected token `;;'");
        else if (arg[i] == '|' && arg[i + 1] == '|')
            error_msg("syntax error near unexpected token `||'");
        else if (arg[i] == ';' && arg[i + 1] == '|')
            error_msg("syntax error near unexpected token `;'");
        else if (arg[i] == '|' && arg[i + 1] == ';')
            error_msg("syntax error near unexpected token `|'");
        if ((arg[i] == ';' || arg[i] == '|') && (arg[i + 1] == ';' || arg[i + 1] == '|'))
            return (1);
        i++;
    }
    return (0);
}
void    fill_args(char *arg)
{
    int     i;
    char    c;
    t_infos infos;
    
    i = 0;
    c = '\0';
    infos.args = ft_split_edited(arg);
    while (infos.args[i])
    {
        printf("arg : %s\n", infos.args[i]);
        i++;
    }
   
}

int     main(int argc, char **argv, char **env)
{
    int i;
    char *str;
    char c;
    char *arg;
    int ret;
    
    (void)argc;
    (void)argv;
    (void)env;
    i = 0;
    str = NULL;
    c = '\0';
    arg = NULL;
    ret = 0;
    while (1)
    {
        arg = malloc(1);
        i = 0;
        ft_putstr(getcwd(str, sizeof(str)));
        ft_putstr("> ");
        while (read(0, &c, 1) > 0)
        {
            if (c == '\n')
                break;
            arg = ft_realloc(arg, i, i + 1);
            arg[i] = c;
            i++;
        }
        arg[i] = '\0';
        // printf("\narg : \"%s\" | len : %zu \n", arg, ft_strlen(arg));
        if (!handling_errors_arg(arg))
            fill_args(arg);
        free(arg);
    }
}