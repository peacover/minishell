/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <yer-raki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 16:08:10 by yer-raki          #+#    #+#             */
/*   Updated: 2021/04/13 17:21:33 by yer-raki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void    error_msg(char *s)
{
    ft_putstr(s);
    ft_putchar('\n');
}
char    *split_char(int start, int end)
{
    int     i;
    char    *s;
    
    i = 0;
    if (start > end)
        return (NULL);
    s = (char *)malloc(sizeof(char) * (end - start + 1));
    // printf ("\n\narg111 = %s\n", g_infos.arg);
    // printf ("\n\nstart = %d\n", start);
    while (g_infos.arg[start] && start <= end)
    {
        s[i] = g_infos.arg[start];
		if (start == end)
        {
            s[i + 1] = '\0';
			return (s);
        }
		i++;
        start++;
    }
    return (NULL);
}

char    *remove_char(char c, int i)
{
    char    *s1;
    char    *s2;

    s1 = NULL;
    s2 = NULL;
    (void)c;
    int t;
    t = (int)ft_strlen(g_infos.arg);
    // if (c == '\\' && !g_infos.arg[i + 1])
    //     error_msg("error multiligne 2");
    // else
    // {
    s1 = split_char(0, i - 1);
    // if ( i != (int)ft_strlen(g_infos.arg) - 1)
    s2 = split_char(i + 1, t - 1);
    // free(g_infos.arg);
    // printf ("\ns1 = %s | s2 = %s\n", s1, s2);
    
    // g_infos.arg = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2)));
    if (s2 == NULL)
        g_infos.arg = ft_strcpy(g_infos.arg, s1);
    else
        g_infos.arg = ft_strjoin(s1, s2);
    free(s1);
    free(s2);
    return (g_infos.arg);
}

int     count_nb_quote(char c)
{
    int i;
    int n;

    i = 0;
    n = 0;
    while (g_infos.arg[i])
    {
        if (g_infos.arg[i] == c)
            n++;
        i++;
    }
    return (n);
}
void    handling_bs_sq_args()
{
    int i;
    int is_bs;
    int l;

    i = 0;
    is_bs = 0;
    l = (int)ft_strlen(g_infos.arg);
    
    if ((count_nb_quote('\'') % 2) != 0)
        error_msg("error multiligne");

    while (g_infos.arg[i])
    {
        if (g_infos.arg[i] == '\'')
        {
            if (i == 0 || (i > 0 && g_infos.arg[i - 1] != '\\'))
            {
                g_infos.arg = remove_char('\'', i);
                l--;
                if (l <= i)
                    break;
                i--;
            } 
        }
        i++;
    }
    
    i = 0;
    while (g_infos.arg[i])
    {
        if (g_infos.arg[i] == '\\') //&& ! (' || ") 
        {
            if (!g_infos.arg[i + 1])
            {
                error_msg("error multiligne");
                break;
            }
            if (is_bs == 1)
            {
                is_bs = 0;
                continue;
            }
            if (g_infos.arg[i + 1] == '\\')
                is_bs = 1;
            g_infos.arg = remove_char('\\', i);
        }
        i++;
    }   
}


int    handling_errors_arg()
{
    int i;

    i = 0;
    while (g_infos.arg[i])
    {
        if (g_infos.arg[i] == ';' && g_infos.arg[i + 1] == ';')
            error_msg("syntax error near unexpected token `;;'");
        else if (g_infos.arg[i] == '|' && g_infos.arg[i + 1] == '|')
            error_msg("syntax error near unexpected token `||'");
        else if (g_infos.arg[i] == ';' && g_infos.arg[i + 1] == '|')
            error_msg("syntax error near unexpected token `;'");
        else if (g_infos.arg[i] == '|' && g_infos.arg[i + 1] == ';')
            error_msg("syntax error near unexpected token `|'");
        if ((g_infos.arg[i] == ';' || g_infos.arg[i] == '|') && (g_infos.arg[i + 1] == ';' || g_infos.arg[i + 1] == '|'))
            return (1);
        i++;
    }
    return (0);
}
void    fill_args()
{
    int     i;
    char    c;
    
    i = 0;
    c = '\0'; 
    handling_bs_sq_args(); // handling backslash and sibgke quotes
    g_infos.args = ft_split_edited(g_infos.arg);
    while (g_infos.args[i])
    {
        printf("g_infos.arg : %s\n", g_infos.args[i]);
        i++;
    }
}

int     main(int argc, char **argv, char **env)
{
    int i;
    char *str;
    char c;
    int ret;
    
    (void)argc;
    (void)argv;
    (void)env;
    i = 0;
    str = NULL;
    c = '\0';
    g_infos.arg = NULL;
    ret = 0;
    while (1)
    {
        g_infos.arg = malloc(1);
        i = 0;
        ft_putstr(getcwd(str, sizeof(str)));
        ft_putstr("> ");
        while (read(0, &c, 1) > 0)
        {
            if (c == '\n')
                break;
            g_infos.arg = ft_realloc(g_infos.arg, i, i + 1);
            g_infos.arg[i] = c;
            i++;
        }
        g_infos.arg[i] = '\0';
        // printf("\narg : \"%s\" | len : %zu \n", g_infos.arg, ft_strlen(g_infos.arg));
        if (!handling_errors_arg())
            fill_args();
        printf ("\n\narg = %s\n", g_infos.arg);
        free(g_infos.arg); 
    }
}