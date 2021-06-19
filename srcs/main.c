/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <yer-raki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 16:08:10 by yer-raki          #+#    #+#             */
/*   Updated: 2021/06/14 10:55:25 by yer-raki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void    error_msg(char *s)
{
    ft_putstr(s);
    ft_putchar('\n');
}
void    print_mylist(t_sep *node)
{
	int i;
	
	i = 0;
	while (node != NULL)
	{
		printf ("\n path : %s", node->path);
		printf ("\n type : %c", node->t_sp);
		i = 0;
		while (node->args && node->args[i])
		{
			printf ("\n arg : %s", node->args[i]);
			i++;
		}
		node = node->next;
	}
}
char    *remove_char(char *s, int i)
{
    char    *s1;
    char    *s2;
	int		l;
	
    l = (int)ft_strlen(s);
	s1 = ft_substr(s, 0, i);
	s2 = ft_substr(s, i + 1, l - i + 1);
	free(s);
	s = ft_strjoin(s1, s2);
    free(s1);
    free(s2);
    return (s);
}

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
    while (current != NULL)
    {
        if (!ft_strcmp(current->key, "PATH"))
        {
            w = ft_split(current->value, ':');
            while (w[i])
            {
                s = ft_strjoin(w[i], "/");
                s = ft_strjoin(s, node->lower_builtin);
                fd = open(s, O_RDONLY);
                if (fd > 0)
                {
                    node->path = s;
					close(fd);
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
    if (!ft_strcmp(node->upper_builtin, "ECHO") || !ft_strcmp(node->upper_builtin, "CD")
    || !ft_strcmp(node->upper_builtin, "PWD") || !ft_strcmp(node->upper_builtin, "EXPORT")
    || !ft_strcmp(node->upper_builtin, "UNSET") || !ft_strcmp(node->upper_builtin, "ENV")
    || !ft_strcmp(node->upper_builtin, "EXIT"))
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

char	*replace_dollar(char *s, char *v, int start, char *key)
{
	char *s1;
	char *s2;
	int l_key;
	int l_s;
	char *w;

	w = NULL;
	l_key = ft_strlen(key);
	l_s = ft_strlen(s);
	s1 = ft_substr(s, 0, start - 1);
	s2 = ft_substr(s, start + l_key, l_s - start - l_key);
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
			// printf ("\n s : |%s|\n", s);
			while (current != NULL)
    		{
				if (!ft_strcmp(current->key, v) || (!ft_strncmp(current->key, v, ft_strlen(current->key))
				&& v[ft_strlen(current->key)] == '\''))
				{
					return (replace_dollar(s, current->value, start, current->key));
				}
				current = current->next;
    		}
			s = ft_substr(s, i, ft_strlen(s) - i);
			// printf ("\n last s : |%s|\n", s);
			return (s);
		}
		i++;
	}
	return (s);
}


void    add_to_args(int start, int end, char *s, int i, t_sep *node)
{
	int		l;
    char	type;
	char 	*str;
	
	node->args[i] = NULL;
    type = s[start];
	str = NULL;
	while (s[start] && start < end)
	{
		if (s[start] != '\'' && s[start] != '\"')
		{
			l = start;
			while (s[l] && s[l] != '\'' && s[l] != '\"' && s[l] != ' ')
				l++;
			str = ft_substr(s, start, l - start);
			start = l;
		}
		else
		{				
			l = search_second_quote(s, start + 1, s[start]);
            if (!l)
                error_msg("error multiligne");
			str = ft_substr(s, start + 1, l - start - 1);
			start = l;
		}
		if (s[start] == '\"')
			str = handling_bs_dq(str);
		else if (s[start] != '\'')
			str = handling_bs(str);
		if (s[start] != '\'')
			str = handling_dollar(str);
		printf("\n\nSTR : %s \\\\ ARG %d : |%s|\n\n", str, i, node->args[i]);
		if (!node->args[i])
		{
			node->args[i] = ft_strdup(str);
			node->args[i] = ft_strcpy(node->args[i], str);
		}
		else
		{
			node->args[i] = ft_strjoin(node->args[i], str);
		}
		printf("\n\nARG %d : |%s|\n\n", i, node->args[i]);
		free(str);
		str = NULL;
	if (s[start] == '\'' || s[start] == '\"')
		start++;
	}
	node->args[i + 1] = NULL;
}

void    get_args(char *s, int start, t_sep *node)
{
    int     i;
    int		end;

    i = 0;
    while (s[start])
    {
		end = start;
        if (s[start] == '\'' || s[start] == '\"')
        {
            if (s[start - 1] == '\\')
                continue;
            end = search_second_quote(s, start + 1, s[start]);
            if (!end)
                error_msg("error multiligne");
            // node->args = ft_realloc_2(node->args, i, (i + 1));
            // add_to_args(start, end, s, i, node);
            // i++;
            // start = end;
        }
        // else
        // {
            // end = start;
            while (s[end] && s[end] != ' ')
                end++;
            if (end > start)
            {
                node->args = ft_realloc_2(node->args, i, (i + 1));
                add_to_args(start, end, s, i, node);
                i++;
            }
            start = end;
        // }
		while (s[start] && s[start] == ' ')
            start++;
    }
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

char	*red_redim_s(char *s, int start, int end)
{
	char	*s1;
	char	*s2;

	s1 = ft_substr(s, 0, start);
	s2 = ft_substr(s, end + 1, ft_strlen(s) - end + 1);
	s = ft_strjoin(s1, " ");
	s = ft_strjoin(s, s2);
	free(s1);
	free(s2);
	// if (type != '\'')
	// {
	// 	if (type == '\"')
	// 		s = handling_bs_dq(s);
	// 	else
	// 		s = handling_bs(s);
    //     s = handling_dollar(s);
	// }
	return(s);
}

char	*red_get_file(t_sep *node, char *s, int start, char type)
{
	int i;
	char *file;
	int end;

	i = 0;
	end = 0;
	file = NULL;
	while (s[start])
	{
		i = start;
		if (type == 'i' || type == 'o' )
			start++;
		else if (type == 'a')
			start += 2;
		while (s[start] && s[start] == ' ')
			start++;
		if (s[start] == '\'' || s[start] == '\"')
        {
            if (s[start - 1] == '\\')
                continue;
            end = search_second_quote(s, start + 1, s[start]);
            if (!end)
                error_msg("error multiligne");
            file = ft_substr(s, start + 1, end - start - 1);
			if (s[start] != '\'')
			{
				if (s[start] == '\"')
					file = handling_bs_dq(file);
				else
					file = handling_bs(file);
			    file = handling_dollar(file);
			}
			node->red_args = red_redim_s(s, i, end);
			// printf("\n FILE : |%s| \n", file);
			return (file);
        }
        else
        {
			end = start;
            while (s[end] && s[end] != ' ' && s[end] != '<' && s[end] != '>')
                end++;
			file = ft_substr(s, start, end - start);
			node->red_args = red_redim_s(s, i, end - 1);
			if (s[start] != '\'')
			{
				if (s[start] == '\"')
					file = handling_bs_dq(file);
				else
					file = handling_bs(file);
			    file = handling_dollar(file);
			}
			// printf("\n FILE : |%s| \n", file);
			return (file);
        }
		return (NULL);
	}
	return (NULL);
}

void    r_add_to_args(int start, int end, char *s, int i, t_sep *node)
{
	int		l;
    char	type;
	
    type = s[start];
    l = end - start;
	node->r_args[i] = NULL;
    if (type != '\'' && type != '\"')
        node->r_args[i] = ft_substr(s, start, l);
    else
	    node->r_args[i] = ft_substr(s, start + 1, l - 1);
	if (type != '\'')
	{
		if (type == '\"')
			node->r_args[i] = handling_bs_dq(node->r_args[i]);
		else
			node->r_args[i] = handling_bs(node->r_args[i]);
        node->r_args[i] = handling_dollar(node->r_args[i]);
	}
	node->r_args[i + 1] = NULL;
	// printf ("\n\n\nred arg %d : |%s|\n", i, node->r_args[i]);
}

void	red_get_cmd_args(t_sep *node)
{
	int     i;
    int		end;
	int start;

    i = 0;
	start = 0;
    while (node->red_args[start])
    {
        if (node->red_args[start] == '\'' || node->red_args[start] == '\"')
        {
            if (node->red_args[start - 1] == '\\')
                continue;
            end = search_second_quote(node->red_args, start + 1, node->red_args[start]);
            if (!end)
                error_msg("error multiligne");
            node->r_args = ft_realloc_2(node->r_args, i, (i + 1));
            r_add_to_args(start, end, node->red_args, i, node);
            i++;
            start = end + 1;
        }
        else
        {
            end = start;
            while (node->red_args[end] && node->red_args[end] != ' ')
                end++;
            if (end > start)
            {
                node->r_args = ft_realloc_2(node->r_args, i, (i + 1));
                r_add_to_args(start, end, node->red_args, i, node);
                i++;
            }
            start = end;
        }
		while (node->red_args[start] && node->red_args[start] == ' ')
            start++;
    }
}

void	red_get_type_file(t_sep *node, char *s, int start)
{
	int		i;
	char	type;
	char	*file;
	int		end;
	int		j;

	i = 1;
	(void)node;
	file = NULL;
	end = 0;
	node->red_args = NULL;
	(void)start;
	j = 0;
	while (s[i])
    {
		while (s[i] && s[i] == ' ')
			i++;
		if (s[i] == '\'' || s[i] == '\"')
        {
            if (s[i - 1] == '\\')
                continue;
            end = search_second_quote(s, i + 1, s[i]);
            if (!end)
                error_msg("error multiligne");
            i = end;
        }
        if ((s[i] == '>' && s[i - 1] != '\\') ||
        (s[i] == '<' && s[i - 1] != '\\') || s[i + 1] == '\0')
        {
			j = i;
			// while (s[start] && s[start] == ' ')
            //     start++;
			// printf ("\n FIRST S : |%s| / START : %d / I : %d\n", s, start, i);
			
			type = red_get_type(s, i);
			file = red_get_file(node, s, i, type);
			s = node->red_args;
			// printf("\n REDIM : |%s| \n", node->red_args);
			printf("\n FILE : |%s| \n", file);
			// red_handling_bs_dollar();
			// printf("\n S : |%s| \n", s);
			i = j;
        }
		if (s[i + 1] == '\0')
			{
				red_get_cmd_args(node);
				printf("\n RED_ARGS : |%s| \n", node->red_args);
				break;
			}
		j = 0;
        i++;
    }
}

void	init_t_sep(t_sep *node)
{
	node->path = NULL;
	node->t_sp = '\0';
	node->args = NULL;
	node->is_builtin = 0;
	node->builtin = NULL;
	node->upper_builtin = NULL;
	node->lower_builtin = NULL;
	node->is_red = 0;
	
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
				red_get_type_file(node, s, start);
				// printf("\ntype red :  %c\n", s[start]);
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
	int		t;

    i = ft_strlen(s);
	t = 0;
    node->builtin = malloc(sizeof(char) * (i + 1));
    node->upper_builtin = malloc(sizeof(char) * (i + 1));
    node->lower_builtin = malloc(sizeof(char) * (i + 1));
    i = 0;
    while (s[i] && s[i] != ' ')
    {
        node->builtin[i] = s[i];
        node->upper_builtin[i] = ft_toupper(s[i]);
        node->lower_builtin[i] = ft_tolower(s[i]);
        i++;
    }
    node->builtin[i] = '\0';
    node->upper_builtin[i] = '\0';
    node->lower_builtin[i] = '\0';
    if (!check_red(node, s) && !check_builtin(node) && !check_fill_path(node))
        error_msg("COMMAND NOT FOUND!!");
	printf("\n\nred return : %d", node-> is_red);
	if (node-> is_red != 1)
	{
		while (s[i] && s[i] == ' ')
			i++;
		get_args(s, i, node);
	}
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
    t_sep	*head;

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
			{
                error_msg("error multiligne");
				break;
			}
            i = end;
        }
        if ((str[i] == '|' && str[i - 1] != '\\') ||
        (str[i] == ';' && str[i - 1] != '\\') || str[i + 1] == '\0')
        {
			while (str[start] && str[start] == ' ')
                start++;
			if (str[i + 1] == '\0')
				s = ft_substr(str, start, i - start + 1);
			else
           		s = ft_substr(str, start, i - start);
            addlast_sep(&head, s, str[i]);
			if (s)
				free(s);
            start = i + 1;
        }
        i++;
    }
	print_mylist(head);
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
}
void	handling_history(char *s)
{
	int		i;
	int		fd;
	char	**history;
	int		l_history;

	i = 0;
	history = NULL;
	history = (char **)malloc(sizeof(char *) * 500);
	while (history[i])
		history[i++] = NULL;
	
	fd = open("./my_bash_history", O_RDWR | O_CREAT, 0644);
	//FILL **HISTORY FROM THE FILE
	l_history = ft_strlen2(history);
	if (l_history < HISTORY_MAX_SIZE)
        history[l_history++] = ft_strdup(s);
    else
	{
        free(history[0]);
		i = 1;
		while (i < HISTORY_MAX_SIZE)
		{
			history[i - 1] = history[i];
			i++;
		}
        history[HISTORY_MAX_SIZE - 1] = ft_strdup(s);
    }
	close(fd);
}

int             ft_puts(int d)
{
        return (write(1, &d, sizeof(int)));
}

int             get_char()
{
        char    c;
        int     total;
        struct termios term, init;
        tcgetattr(0, &term);
        tcgetattr(0, &init);
        term.c_lflag &= ~(ICANON | ECHO);
        term.c_cc[VMIN] = 0;
        term.c_cc[VTIME] = 0;
        tcsetattr(0, TCSANOW, &term); // ??
        total = 0;
        while (read(0, &c, 1) <= 0);
        total += c;
        while (read(0, &c, 1) > 0)
                total += c;
        tcsetattr(0, TCSANOW, &init); // ??
        return (total);
}

int     main(int argc, char **argv, char **env)
{
    int i;
	int d;
    char *str;
	char *pwd;
    int ret;
	char c;

    (void)argc;
    (void)argv;
    g_env = fill_env(env);
    // print_list_env();
    i = 0;
    ret = 0;
	d = 0;
	str = NULL;
	pwd = NULL;
	pwd = getcwd(pwd, 0);
    while (1)
    {
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
			// handling_history(str);
        }
        if (!handling_errors_arg(str))
            fill_args(str);
        printf ("\n\narg = %s\n", str);
		// ft_putstr(str);
		// printf ("\n\nc = %s\n", str);
		// if (str)
        // 	free(str);
		// system("leaks minishell");
    }
}