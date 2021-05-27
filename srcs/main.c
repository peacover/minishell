/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <yer-raki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 16:08:10 by yer-raki          #+#    #+#             */
/*   Updated: 2021/05/25 16:16:06 by yer-raki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void    error_msg(char *s)
{
    ft_putstr(s);
    ft_putchar('\n');
}
char    *split_char(char *s, int start, int end)
{
    int     i;
    char    *str;
    
    i = 0;
    if (start > end)
        return (NULL);
    str = (char *)malloc(sizeof(char) * (end - start + 1));
    // printf ("\n\narg111 = %str\n", g_inf.arg);
    // printf ("\n\nstart = %d\n", start);
    while (s[start] && start <= end)
    {
        str[i] = s[start];
		if (start == end)
        {
            str[i + 1] = '\0';
			return (str);
        }
		i++;
        start++;
    }
    return (NULL);
}

char    *remove_char(char *s, int i)
{
    char    *s1;
    char    *s2;
	int		l;
	
    l = (int)ft_strlen(s);
	s1 = malloc(sizeof(char) * (i + 1));
	s2 = malloc(sizeof(char) * (l - i + 1));
	s1 = ft_substr(s, 0, i);
	s2 = ft_substr(s, i + 1, l - i + 1);
    
    // printf ("\ns1 : |%s|\n", s1);
    // printf ("\ns2 : |%s|\n", s2);
    
    // s1 = split_char(s, 0, i - 1);
    // s2 = split_char(s, i + 1, l - 1);
    // if (s2 == NULL)
        //  s = ft_strcpy(s, s1);
    // else
    free (s);
    s = ft_strjoin(s1, s2);
    free(s1);
    free(s2);
    return (s);
}

// int     count_nb_quote(char c)
// {
//     int i;
//     int n;

//     i = 0;
//     n = 0;
//     while (g_inf.arg[i])
//     {
//         if (g_inf.arg[i] == c)
//             n++;
//         i++;
//     }
//     return (n);
// }
/*void    handling_bs_sq_args()
{
    int i;
    int is_bs;
    int l;

    i = 0;
    is_bs = 0;
    l = (int)ft_strlen(g_inf.arg);
    
    if ((count_nb_quote('\'') % 2) != 0)
        error_msg("error multiligne");
    while (g_inf.arg[i])
    {
        if (g_inf.arg[i] == '\'')
        {
            if (i == 0 || (i > 0 && g_inf.arg[i - 1] != '\\'))
            {
                g_inf.arg = remove_char('\'', i);
                l--;
                if (l <= i)
                    break;
                i--;
            } 
        }
        i++;
    }

    i = 0;
    while (g_inf.arg[i])
    {
        if (g_inf.arg[i] == '\\') //&& ! (' || ") 
        {
            if (!g_inf.arg[i + 1])
            {
                error_msg("error multiligne");
                break;
            }
            if (is_bs == 1)
            {
                is_bs = 0;
                continue;
            }
            if (g_inf.arg[i + 1] == '\\')
                is_bs = 1;
            g_inf.arg = remove_char('\\', i);
        }
        i++;
    }   
}*/

int    handling_errors_arg(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] == ';' && str[i + 1] == ';')
            error_msg("syntax error near unexpected token `;;'");
        else if (str[i] == '|' && str[i + 1] == '|')
            error_msg("syntax error near unexpected token `||'");
        else if (str[i] == ';' && str[i + 1] == '|')
            error_msg("syntax error near unexpected token `;'");
        else if (str[i] == '|' && str[i + 1] == ';')
            error_msg("syntax error near unexpected token `|'");
        if ((str[i] == ';' || str[i] == '|') && (str[i + 1] == ';' || str[i + 1] == '|'))
            return (1);
        i++;
    }
    return (0);
}

int     check_fill_path(t_sep *node)
{
    int     i;
    char    **w;
    char    *s;
    int     fd;
    
    i = 0;
    fd = 0;
    t_env *current = g_env;
    // printf ("\ntttttttttttttttt\n");
    while (current != NULL)
    {
        if (!ft_strcmp(current->key, "PATH"))
        {
            w = ft_split(current->value, ':');
            while (w[i])
            {
                s = ft_strjoin(w[i], "/");
                s = ft_strjoin(s, node->cmd.lower_builtin);
                // printf ("\n s = |%s| \n", s);
                fd = open(s, O_RDONLY);
                if (fd >= 0)
                {
                    node->path = s;
                    return (1);
                }
                i++;
            }
        }
        current = current->next;
    }
    return (0);
}

int    check_builtin(t_sep *node)
{
    int i;

    i = 0;
    if (!ft_strcmp(node->cmd.upper_builtin, "ECHO") || !ft_strcmp(node->cmd.upper_builtin, "CD")
    || !ft_strcmp(node->cmd.upper_builtin, "PWD") || !ft_strcmp(node->cmd.upper_builtin, "EXPORT")
    || !ft_strcmp(node->cmd.upper_builtin, "UNSET") || !ft_strcmp(node->cmd.upper_builtin, "ENV")
    || !ft_strcmp(node->cmd.upper_builtin, "EXIT"))
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

int     search_second_quote(char *s, int start, char type)
{
    if (type == '\'')
    {
        while (s[start])
        {
            if (s[start] == type)
                return (start);
            start++;
        }
        return (0);
    }
    else if (type == '\"')
    {
        while (s[start])
        {
            if (s[start] == type && s[start]!= '\\')
                return (start);
            start++;
        }
        return (0);
    }
	return (0);
}

char	*handling_bs(char *s)
{
	int i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\\')
		{
            // printf ("\ns : |%s|\n", s);
			s = remove_char(s, i);
			i++;
		}
		i++;
	}
    // printf ("\ns : |%s|\n", s);
    return (s);
}

void    add_to_args(int start, int end, char *s, int i, t_sep *node)
{
	int		l;
    char	type;

    type = s[start];
    l = end - start;
    // printf ("\nstart : %d | end : %d\n", start, end);
	
    if (type != '\'' && type != '\"')
	{
		node->cmd.args[i] = malloc(sizeof(char) * (l + 1));
        node->cmd.args[i] = ft_substr(s, start, l);
	}
    else
	{
		node->cmd.args[i] = malloc(sizeof(char) * (l));
	    node->cmd.args[i] = ft_substr(s, start + 1, l - 1);
	}
    // printf ("\ns : |%s|\n", node->cmd.args[i]);
	if (type != '\'')
	{
		node->cmd.args[i] = ft_strcpy(node->cmd.args[i], handling_bs(node->cmd.args[i]));
        
		// TODO : HANDLING DOLLAR
	}
	printf ("\narg %d : |%s|\n", i, node->cmd.args[i]);
}

void    get_args(char *s, int start, t_sep *node)
{
    int     i;
    int		end;

    i = 0;
    while (s[start])
    {
        if (s[start] == '\'' || s[start] == '\"')
        {
            if (s[start - 1] == '\\')
                continue;
            end = search_second_quote(s, start + 1, s[start]);
            if (!end)
                error_msg("error multiligne");
            node->cmd.args = ft_realloc(node->cmd.args, i, (i + 1));
            add_to_args(start, end, s, i, node);
            i++;
            start = end + 1;
        }
        else
        {
            while (s[start] && s[start] == ' ')
                start++;
            if (s[start] == '\0')
                break;
            end = start;
            while (s[end] && s[end] != ' ')
                end++;
            if (end > start)
            {
                node->cmd.args = ft_realloc(node->cmd.args, i, (i + 1));
                add_to_args(start, end, s, i, node);
                i++;
            }
            start = end + 1;
        }
    }
    
	// i = 0;
	// while (node->cmd.args[i])
	// {
	// 	printf ("\n arg %d : |%s|\n", i, node->cmd.args[i]);
	// 	i++;
	// }
}

// int		check_redirection(char *s, int i)
// {
// 	while (s[i])
// 	{
// 		if (s[])
// 	}
// }

// void	check_handle_redirections(char *s, int i, t_sep *node)
// {
// 	check_redirection(s, i);
// }

void    get_builtin(char *s, t_sep *node)
{
    int     i;

    i = 0;
    while (s[i] && s[i] == ' ')
        i++;
    node->cmd.builtin = malloc(sizeof(char) * (i + 1));
    node->cmd.upper_builtin = malloc(sizeof(char) * (i + 1));
    node->cmd.lower_builtin = malloc(sizeof(char) * (i + 1));
    i = 0;
    while (s[i] && s[i] != ' ')
    {
        node->cmd.builtin[i] = s[i];
        node->cmd.upper_builtin[i] = ft_toupper(s[i]);
        node->cmd.lower_builtin[i] = ft_tolower(s[i]);
        i++;
    }
    node->cmd.builtin[i] = '\0';
    node->cmd.upper_builtin[i] = '\0';
    node->cmd.lower_builtin[i] = '\0';
    
    if (!check_builtin(node) && !check_fill_path(node))
        error_msg("COMMAND NOT FOUND!!");
    // printf ("\n path : |%s| \n", node->path);
    while (s[i] && s[i] == ' ')
        i++;
    // check_handle_redirections(s, i, node);
    get_args(s, i, node);
}

void    fill_node(char *s, t_sep *node, char type)
{
    // handling_bs_sq_args();
    int i;

    i = 0;
    node->t_sp = type;
    get_builtin(s, node);
    // printf ("\nlower : %s", node->cmd.lower_builtin);
    
    // if (!check_builtin(node))
    //     error_msg("there is no builtin or buitin not handled!!");

    
    // while (node->cmd.args[i])
    // {
    //     printf("\narg %d : |%s|\n", i, node->cmd.args[i]);
    //     i++;
    // }
}

void addLast(t_sep **head, char *s, char type)
{
    t_sep *newNode = malloc(sizeof(t_sep));
    
    fill_node(s, newNode, type);
    newNode->next = NULL;
    if (*head == NULL)
         *head = newNode;
    else
    {
        t_sep *lastNode = *head;
        while (lastNode->next != NULL)
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
        printf("\nbuiltin : %s\n", node->cmd.builtin);
        node = node->next;
    }
}

void    fill_list(char *str)
{
    int     i;
    int     start;
    char    *s;
    int     l;
    t_sep *head;

    i = 1;
    start = 0;
    head = NULL;
    l = ft_strlen(str);
    while (str[i])
    {
        if ((str[i] == '|' && str[i - 1] != '\\') ||
        (str[i] == ';' && str[i - 1] != '\\') || str[i + 1] == '\0')
        {
            if (str[i + 1] == '\0')
                i++;
            while (str[start] == ' ')
                start++;
            s = ft_substr(str, start, i - start);
            // printf ("\n s : |%s|\n", s);
            addLast(&head, s, str[i]);
            start = i + 1;
        }
        i++;
    }
    // free(s);
    //print_mylist(head);
}

void    fill_args(char *str)
{
    int     i;
    
    i = 0;
    fill_list(str);
    // handling_bs_sq_args(); // handling backslash and single quotes
    // printf("\n my arg = %s \n", g_inf.arg);
    // printf("\n my arg2 = %s \n", g_inf.or_arg);

}


void    print_list_env()
{
    t_env *current = g_env;
    while (current != NULL)
    {
        
        //printf("declare -x ");
        printf("%s\n",current->key);
        //printf("=\"");
        //printf("%s\"\n",current->value);
        current = current->next;
    }
}

t_env    *fill_env(char **env)
{
    t_env *head = NULL;
    t_env *tail = NULL;
    int i;
    i = 0;
    t_env *temp;
    char **s;
    while (env[i])
    {
        s = ft_split(env[i],'=');
        temp = malloc(sizeof(*temp));
        temp->val = env[i];
        temp->key = s[0];
        temp->value= s[1];
        temp->next = NULL;
        if (!head)
        {
            head = temp;
            tail = head;
        }
        else
        {
            tail->next = temp;
            tail = temp;
        }
        i++;
    }
    return (head);
    // print_list_env();
}

int     main(int argc, char **argv, char **env)
{
    int i;
    char *str;
    char c;
    int ret;
    
    (void)argc;
    (void)argv;
    g_env = fill_env(env);
    print_list_env();
    i = 0;
    ret = 0;
    // str = NULL;
    while (1)
    {
        // g_inf.arg = malloc(1);
        str = NULL;
        i = 0;
        // handle echo -n case !!
        ft_putstr(getcwd(str, sizeof(str)));
        ft_putstr("> ");
        // printf ("\n c = %zd \n", read(0, &c, 1));
        while (read(0, &c, 1) > 0)
        {
            if (c == '\n')
                break;
            str = ft_realloc(str, i, i + 1);
            str[i] = c;
            i++;
        }
        str[i] = '\0';
        // printf("\narg : \"%s\" | len : %zu \n", g_inf.arg, ft_strlen(g_inf.arg));
        if (!handling_errors_arg(str)) // handling | and ; errors 
            fill_args(str);
        printf ("\n\narg = %s\n", str);
        free(str);
        // free(g_inf.arg);
        // free(g_inf.or_arg);
    }
}