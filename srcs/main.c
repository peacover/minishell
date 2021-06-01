/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <yer-raki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 16:08:10 by yer-raki          #+#    #+#             */
/*   Updated: 2021/06/01 15:50:29 by yer-raki         ###   ########.fr       */
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

void	print_args(char **w)
{
	int i;

	i = 0;
	while (w[i])
	{
		printf("\narg %d : |%s|\n", i, w[i]);
		i++;
	}
}

char    *remove_char(char *s, int i)
{
    char    *s1;
    char    *s2;
	int		l;
	
    l = (int)ft_strlen(s);
	// s1 = malloc(sizeof(char) * (i + 1));
	// s2 = malloc(sizeof(char) * (l - i + 1));
	s1 = ft_substr(s, 0, i);
	s2 = ft_substr(s, i + 1, l - i + 1);
    
    // printf ("\ns1 : |%s|\n", s1);
    // printf ("\ns2 : |%s|\n", s2);
    
    // s1 = split_char(s, 0, i - 1);
    // s2 = split_char(s, i + 1, l - 1);
    // if (s2 == NULL)
    //      s = ft_strcpy(s, s1);
    // else
	// {
	free(s);
	s = ft_strjoin(s1, s2);
	// }
    free(s1);
    free(s2);
	// printf ("\n s : |%s|", s);
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
	if (!str)
		return (1);
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
	s = NULL;
    t_env *current = g_env;
	// node->path = NULL;
    while (current != NULL)
    {
        if (!ft_strcmp(current->key, "PATH"))
        {
            w = ft_split(current->value, ':');
            while (w[i])
            {
                s = ft_strjoin(w[i], "/");
                s = ft_strjoin(s, node->cmd.lower_builtin);
                fd = open(s, O_RDONLY);
                if (fd > 0)
                {
                    node->path = s;
					close(fd);
					// free(s);
					// printf (" \npath : %s \n", node->path);
                    return (1);
                }
				close(fd);
				free(s);
                i++;
            }
        }
        current = current->next;
    }
	// printf (" \npath : %s \n", node->path);
	// free w
	node->path = NULL;
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
			if (!s[i + 1])
			{
				error_msg("error multiligne");
                break;
			}
			s = remove_char(s, i);
		}
		i++;
	}
    return (s);
}

char	*handling_bs_dq(char *s)
{
	int i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\\')
		{
			if (!s[i + 1])
			{
				error_msg("error multiligne");
                break;
			}
			if (s[i + 1] == '\\')
				s = remove_char(s, i);
		}
		i++;
	}
    return (s);
}

char	*replace_dollar(char *s, char *v, int start, int end)
{
	char *s1;
	char *s2;
	char *w;

	w = NULL;
	s1 = ft_substr(s, 0, start - 1);
	s2 = ft_substr(s, end, ft_strlen(s) - end);
	w = ft_strjoin(s1, v);
	w = ft_strjoin(w, s2);
	return (w);
}

char	*handling_dollar(char *s)
{
	int		i;
	int		start;
	char	*v;
	t_env	*current = g_env;
   
	i = 0;
	start = 0;
	while (s[i])
	{
		if (s[i] == '$')
		{
			start = ++i;
			while (s[i] && s[i] != ' ')
				i++;
			v = ft_substr(s, start, i - start);
			while (current != NULL)
    		{
				if (!ft_strcmp(current->key, v))
					return (replace_dollar(s, current->value, start, i));
				current = current->next;
    		}
		}
		i++;
	}
	return (s);
}


void    add_to_args(int start, int end, char *s, int i, t_sep *node)
{
	int		l;
    char	type;
	
    type = s[start];
    l = end - start;
    if (type != '\'' && type != '\"')
        node->cmd.args[i] = ft_substr(s, start, l);
    else
	    node->cmd.args[i] = ft_substr(s, start + 1, l - 1);
	if (type != '\'')
	{
		if (type == '\"')
			node->cmd.args[i] = handling_bs_dq(node->cmd.args[i]);
		else
			node->cmd.args[i] = handling_bs(node->cmd.args[i]);
        node->cmd.args[i] = handling_dollar(node->cmd.args[i]);
	}
	printf ("\narg %d : |%s|\n", i, node->cmd.args[i]);
}

void    get_args(char *s, int start, t_sep *node)
{
    int     i;
    int		end;

    i = 0;
	node->cmd.args = NULL;
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
            end = start;
            while (s[end] && s[end] != ' ')
                end++;
            if (end > start)
            {
                node->cmd.args = ft_realloc(node->cmd.args, i, (i + 1));
                add_to_args(start, end, s, i, node);
                i++;
            }
            start = end;
        }
		while (s[start] && s[start] == ' ')
            start++;
    }
	// print_args(node->cmd.args);
    // print_args(node->cmd.args);
	/*i = 0;
	while (node->cmd.args[i])
	{
		printf ("\n arg %d : |%s|\n", i, node->cmd.args[i]);
		i++;
	}*/
}

char		check_redirection(char *s, int i)
{
	while (s[i])
	{
		if (s[i] == '>')
		{
			if (s[i] && s[i + 1] == '>')
				return ('a');
			return ('o');
		}
		else if (s[i] == '<')
		{
			if (s[i] && s[i + 1] == '<')
				error_msg("redirection not handled!!");
			else
				return ('i');
		}
	}
	return (0);
}

void	fill_node_red(char *s, t_red *newNode, char type)
{
	newNode->type = type;
	printf ("\ns : %s", s);
	printf ("\ntype : %c\n", newNode->type);
}

void	addlast_red(t_red **head, char *s, char type)
{
	int i;
	
	i = 0;
    t_red *newNode = malloc(sizeof(t_red));
    t_red *lastNode = *head;
    fill_node_red(s, newNode, type);
    newNode->next = NULL;
    if (*head == NULL)
         *head = newNode;
    else
    {
        lastNode = *head;
        while (lastNode->next != NULL)
            lastNode = lastNode->next;
        lastNode->next = newNode;
    }
}

// void	handle_redirections(char *s, t_red *node)
// {
		
// 	addlast_red(&node->red, s, type);
	
// }

char	red_get_type(char *s, int start)
{
	int i;

	i = 0;
	while (s[start])
	{
		if (s[start] == '>')
		{
			if (s[start + 1] == '>')
			{
				if (s[start + 2] == '>')
					error_msg("syntax error near unexpected token `>'");
				return ('a');
			}
			return ('o');
		}
		if (s[start] == '<')
		{
			if (s[start + 1] == '<')
			{
				if ((s[start + 2] == '<' && s[start + 3] != '<') ||
				(s[start + 2] == '<' && s[start + 3] == '<'))
					error_msg("syntax error near unexpected token `<'");
				error_msg("REDIRECTION NOT HANDLED!");
			}
			return ('i');
		}
		start++;
	}
	return('\0');
}

void	red_get_type_file(char *s, int start)
{
	int		i;
	char	*v;
	char	type;

	i = 0;
	v = ft_strdup(s);
	type = red_get_type(s, start);
	printf("type : %c", type);
	// while (s[start])
	// {
	// 	start++;
	// }
}
void	init_t_sep(t_sep *node)
{
	node->path = NULL;
	node->t_sp = '\0';
	node->cmd.args = NULL;
	node->cmd.is_builtin = 0;
	node->cmd.builtin = NULL;
	node->cmd.upper_builtin = NULL;
	node->cmd.lower_builtin = NULL;
}
int		check_red(t_sep *node, char *s)
{
	int start;
	int end;

	start = 0;
	end = 0;
	while (s[start])
	{
		if (s[start] == '\'' || s[start] == '\"')
        {
            if (s[start - 1] == '\\')
                continue;
            end = search_second_quote(s, start + 1, s[start]);
            if (!end)
                error_msg("error multiligne");
            start = end;
        }
		else
		{
			if ((start == 0 && (s[start] == '>' || s[start] == '<')) ||
			(start > 0 && (s[start] == '>' || s[start] == '<') && s[start - 1] != '\\'))
			{
				node->is_red = 1;
				red_get_type_file(s, start);
				return (1);
			}
		}
		start++;
	}
	return (0);
}

void    get_builtin(char *s, t_sep *node)
{
    int     i;
	int t;

    i = ft_strlen(s);
	t = 0;
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
    if (!check_builtin(node) && !check_fill_path(node) && !check_red(node, s))
        error_msg("COMMAND NOT FOUND!!");
    printf (" \npath : %s \n", node->path);
	// if (!check_red)
	// 	handle_redirections(s, node->red);
	// else
	// {
	while (s[i] && s[i] == ' ')
		i++;
	get_args(s, i, node);
	// }
}

void    fill_node(char *s, t_sep *node, char type)
{
    int i;

    i = 0;
    node->t_sp = type;
    get_builtin(s, node);
    // printf ("\nlower : %s", node->cmd.lower_builtin);
    
    // if (!check_builtin(node))
    //     error_msg("there is no builtin or buitin not handled!!");
}


void	addlast_sep(t_sep **head, char *s, char type)
{
	int i;
	
	i = 0;
    t_sep *newNode = malloc(sizeof(t_sep));
    t_sep *lastNode = *head;
	init_t_sep(newNode);
    fill_node(s, newNode, type);
	// printf ("\ntype : %c \n", newNode->t_sp);
	// printf ("\n path : %s", newNode->path);
	
//EXEC   
    newNode->next = NULL;
    if (*head == NULL)
         *head = newNode;
    else
    {
        lastNode = *head;
        while (lastNode->next != NULL)
        {
            lastNode = lastNode->next;
        }
        lastNode->next = newNode;
    }
	
}

void    print_mylist(t_sep *node)
{
	int i;
	
	i = 0;
	while (node != NULL)
	{
		// printf ("\n path : %s", node->path);
		printf ("\n type : %c", node->t_sp);

		// printf ("\n arg : %s", node->cmd.args[0]);
		
		node = node->next;
	}

}

void	free_mylist_red(t_red *head)
{
	t_red *tmp;
	
	while (head != NULL)
	{
       tmp = head;
       head = head->next;
       free(tmp);
    }
}

void	free_mylist_sep(t_sep *head)
{
	t_sep *tmp;
	
	while (head != NULL)
	{
       tmp = head;
       head = head->next;
       free(tmp);
    }
}



void	fill_list(char *str)
{
    int     i;
    int     start;
    char    *s;
    int     l;
	int		end;
    t_sep *head;

    i = 1;
	end = 0;
    start = 0;
    head = NULL;
    l = ft_strlen(str);
	if (l == 1)
		error_msg("COMMAND NOT FOUND!");
    while (str[i])
    {
		if (str[i] == '\'' || str[i] == '\"')
        {
            if (str[i - 1] == '\\')
                continue;
            end = search_second_quote(str, i + 1, str[i]);
            if (!end)
                error_msg("error multiligne");
            i = end;
        }
        if ((str[i] == '|' && str[i - 1] != '\\') ||
        (str[i] == ';' && str[i - 1] != '\\') || str[i + 1] == '\0')
        {
			// init_t_sep(head);
			while (str[start] && str[start] == ' ')
                start++;
			if (str[i + 1] == '\0')
				s = ft_substr(str, start, i - start + 1);
			else
           		s = ft_substr(str, start, i - start);
            addlast_sep(&head, s, str[i]);
			// printf("\n type : |%c| \n", head->t_sp);
			// printf (" \npath : %s \n", head->path);
			
//EXEC !!
			
			if (s)
				free(s);
			// free(temp->cmd.args);
			// printf ("\n start : %d", start);
            start = i + 1;
        }
        i++;
    }
	// print_mylist(head);
    // free(s);
    // print_mylist(head);
	// FUNCTIONS .....
	// free_mylist_red(head->red);
	// free_mylist_sep(head);
	
}

void    fill_args(char *str)
{
    int     i;
    
    i = 0;
    fill_list(str);
}


void    print_list_env()
{
    t_env *current = g_env;
    while (current != NULL)
    {
        printf("%s\n",current->key);
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
	char *pwd = NULL;
    char c;
    int ret;
    
    (void)argc;
    (void)argv;
    g_env = fill_env(env);
    // print_list_env();
    i = 0;
    ret = 0;
	pwd = getcwd(pwd, 0);
    while (1)
    {
		str = NULL;
        i = 0;
        ft_putstr(pwd);
        ft_putstr("> ");
        while (read(0, &c, 1) > 0)
        {
            if (c == '\n')
                break;
            str = ft_realloc(str, i, i + 2);
            str[i] = c;
            i++;
			str[i] = '\0';
        }
        if (!handling_errors_arg(str))
            fill_args(str);
        printf ("\n\narg = %s\n", str);
		if (str)
        	free(str);
		// system("leaks minishell");
    }
}