/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <yer-raki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 16:08:10 by yer-raki          #+#    #+#             */
/*   Updated: 2021/05/21 11:10:34 by yer-raki         ###   ########.fr       */
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

    (void)c;
    int t;
    t = (int)ft_strlen(g_infos.arg);
    s1 = split_char(0, i - 1);
    s2 = split_char(i + 1, t - 1);
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

int    check_builtin(t_sep *node)
{
    int i;

    i = 0;
    if (!ft_strcmp(node->cmd.norm_builtin, "echo") || !ft_strcmp(node->cmd.norm_builtin, "cd") || !ft_strcmp(node->cmd.norm_builtin, "pwd")
    || !ft_strcmp(node->cmd.norm_builtin, "export") || !ft_strcmp(node->cmd.norm_builtin, "unset") || !ft_strcmp(node->cmd.norm_builtin, "env") || !ft_strcmp(node->cmd.norm_builtin, "exit"))
    {
      return (1);
    }
    return (0);
    // printf ("\n s : |%s| \n", s);

    // if (!ft_strcmp(s, "echo") && (s[4] == ' ' || s[4] == '\0'))
    // {
    //     node->cmd.is_builtin = 1;
    //     node->builtin = ft_substr(s, 0, 4);
    //     //printf("\nbuiltin : %s\n", node->builtin);
    // }
    
}

void    get_builtin(char *s, t_sep *node)
{
    int     i;

    i = 0;
    while (s[i] && s[i] != ' ')
        i++;
    node->cmd.builtin = malloc(sizeof(char) * (i + 1));
    node->cmd.norm_builtin = malloc(sizeof(char) * (i + 1));
    i = 0;
    while (s[i] && s[i] != ' ')
    {
        node->cmd.builtin[i] = s[i];
        node->cmd.norm_builtin[i] = ft_tolower(s[i]);
        i++;
    }
    node->cmd.builtin[i] = '\0';
    node->cmd.norm_builtin[i] = '\0';
}

void    fill_node(char *s, t_sep *node)
{
    // handling_bs_sq_args();
    int i;

    i = 0;
    get_builtin(s, node);
    printf("\nbuilting : |%s|\n", node->cmd.builtin);
    if (!check_builtin(node))
        error_msg("there is no builtin or buitin not handled!!");
    node->cmd.args = ft_split(s, ' ');
    while (node->cmd.args[i])
    {
        printf("\narg %d : |%s|\n", i, node->cmd.args[i]);
        i++;
    }
}

void addLast(t_sep **head, char *s, char type)
{
    t_sep *newNode = malloc(sizeof(t_sep));

    type = ';';
    fill_node(s, newNode);
    newNode->next = NULL;
    if(*head == NULL)
         *head = newNode;
    else
    {
        t_sep *lastNode = *head;
        while(lastNode->next != NULL)
        {
            lastNode = lastNode->next;
        }
        lastNode->next = newNode;
    }
}

void    print_mylist(t_sep *node)
{
    while (node)
    {
        printf("\nbuiltin : %s\n", node->builtin);
        node = node->next;
    }
}

void    fill_list()
{
    int     i;
    int     start;
    char    *s;
    int     l;
    t_sep *head;

    i = 1;
    start = 0;
    head = NULL;
    l = ft_strlen(g_infos.arg);
    while (g_infos.arg[i])
    {
        if ((g_infos.arg[i] == '|' && g_infos.arg[i - 1] != '\\') ||
        (g_infos.arg[i] == ';' && g_infos.arg[i - 1] != '\\') || g_infos.arg[i + 1] == '\0')
        {
            if (g_infos.arg[i + 1] == '\0')
                i++;
            while (g_infos.arg[start] == ' ')
                start++;
            s = ft_substr(g_infos.arg, start, i - start);
            printf ("\n s : |%s|\n", s);
            addLast(&head, s, g_infos.arg[i]);
            start = i + 1;
        }
        i++;
    }
    // free(s);
    //print_mylist(head);
}

void    fill_args()
{
    int     i;
    char    c;
    
    i = 0;
    c = '\0';
    
    fill_list();
    // handling_bs_sq_args(); // handling backslash and single quotes
    // printf("\n my arg = %s \n", g_infos.arg);
    // printf("\n my arg2 = %s \n", g_infos.or_arg);

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
        // handle echo -n case !!
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
        g_infos.or_arg = ft_substr(g_infos.arg, 0, ft_strlen (g_infos.arg));
        // printf("\narg : \"%s\" | len : %zu \n", g_infos.arg, ft_strlen(g_infos.arg));
        if (!handling_errors_arg()) // handling | and ; errors 
            fill_args();
        printf ("\n\narg = %s\n", g_infos.arg);
        free(g_infos.arg);
        free(g_infos.or_arg);
    }
}