/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <yer-raki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 16:08:10 by yer-raki          #+#    #+#             */
/*   Updated: 2021/11/15 11:39:59 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

void	error_msg(char *s)
{
	ft_putstr(s);
	ft_putchar('\n');
	exit(0);
}

char	*join_free(char *s1, char *s2)
{
	char	*tmp;

	tmp = ft_strjoin(s1, s2);
	free(s1);
	return (tmp);
}

void	print_red(t_red *red)
{
	int i;

	i = 0;
	while (red)
	{
		printf(" \nRED_Node: %d\n", i);
		printf("\n---------------------------\n");
		printf (" File : %s", red->r_file);
		printf ("\n Type_red : %c", red->red_op);
		printf("\n---------------------------\n");
		red = red->next;
		i++;
	}
}
void    print_mylist(t_sep *node, int pipes_num)
{
	int i;
	int l;
	int l2;
	int node_num;

	i = 0;
	node_num = 0;
	while (node != NULL)
	{
		printf(" \nNode: %d\n", node_num);
		printf("\n------------------------------------------\n");
		printf (" path : %s", node->path);
		printf ("\n cmd : %s", node->builtin);
		printf ("\n is_builtin : %d", node->is_builtin);
		printf ("\n builtin : %s", node->builtin);
		printf ("\n s_red : %s", node->s_red);
		printf ("\n sep : %c", node->t_sp);
		printf ("\n pipes_num : %d", pipes_num);

		printf ("\n\n is_red : %d", node->is_red);
		print_red(node->red);
		i = 0; 
		l = ft_strlen2(node->args);
		l2 = ft_strlen2(node->r_args);
		while (l > i)
		{
			printf ("\n arg %d : |%s|", i, node->args[i]);
			i++;
		}
		// while (l2 > i)
		// {
		// 	printf ("\n r_arg %d : |%s|", i, node->r_args[i]);
		// 	i++;
		// }
		printf("\n------------------------------------------\n\n");
		node_num++;
		node = node->next;
	}
}

char	*remove_char(char *s, int i)
{
	char	*s1;
	char	*s2;
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

int	handling_errors_arg(char *str)
{
	int	i;

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
		if ((str[i] == ';' || str[i] == '|')
			&& (str[i + 1] == ';' || str[i + 1] == '|'))
			return (1);
		i++;
	}
	return (0);
}

int	check_fill_path2(char *w, t_sep *node)
{
	int		fd;
	char	*s;

	fd = 0;
	s = ft_strjoin(w, "/");
	s = join_free(s, node->lower_builtin);
	fd = open(s, O_RDONLY);
	if (fd > 0)
	{
		node->path = s;
		close(fd);
		return (1);
	}
	close(fd);
	free(s);
	return (0);
}

void	check_fill_path(t_sep *node)
{
	int		i;
	char	**w;
	t_env	*current;

	i = 0;
	current = g_data.envl;
	while (current != NULL)
	{
		if (!ft_strcmp(current->key, "PATH"))
		{
			w = ft_split(current->value, ':');
			while (w[i])
			{
				if (check_fill_path2(w[i], node))
					return ;
				i++;
			}
		}
		current = current->next;
	}
	node->path = NULL;
}

void	check_builtin(t_sep *node)
{
	int	i;

	i = 0;
	if (!ft_strcmp(node->upper_builtin, "ECHO")
		|| !ft_strcmp(node->upper_builtin, "CD")
		|| !ft_strcmp(node->upper_builtin, "PWD")
		|| !ft_strcmp(node->upper_builtin, "EXPORT")
		|| !ft_strcmp(node->upper_builtin, "UNSET")
		|| !ft_strcmp(node->upper_builtin, "ENV")
		|| !ft_strcmp(node->upper_builtin, "EXIT"))
	{
		node->is_builtin = 1;
	}
}

int	search_sq(char *s, int start, char type)
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
			if (s[start] == type && s[start] != '\\')
				return (start);
			start++;
		}
		return (0);
	}
	return (0);
}

char	*handling_bs(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\\')
		{
			if (!s[i + 1])
			{
				error_msg("error multiligne");
				break ;
			}
			s = remove_char(s, i);
		}
		i++;
	}
	return (s);
}

char	*handling_bs_dq(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\\')
		{
			if (!s[i + 1])
			{
				error_msg("error multiligne");
				break ;
			}
			if (s[i + 1] == '\\')
				s = remove_char(s, i);
		}
		i++;
	}
	return (s);
}

int	equal_export(char *s, int i)
{
	i++;
	while (s[i] && s[i] != '$' && (ft_isalpha(s[i]) || s[i] == '_'
			|| ft_isdigit(s[i]) || (s[i] == '?' && s[i - 1] == '$')))
		i++;
	return (i);
}

char	*str_export_split(char *s, int start, int is_dollar)
{
	int	i;

	i = start;
	if (is_dollar)
	{
		while (s[i] && s[i] != '$' && (ft_isalpha(s[i]) || s[i] == '_'
				|| ft_isdigit(s[i]) || (s[i] == '?' && s[i - 1] == '$')))
			i++;
		return (ft_substr(s, start, i - start));
	}
	else
	{
		while (s[i] && s[i] != '$')
			i++;
		return (ft_substr(s, start, i - start));
	}
}

int	check_dollar(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '$')
			return (1);
		i++;
	}
	return (0);
}

void	handling_dollar3(char *s, t_vars *vars)
{
	vars->s1 = str_export_split(s, vars->i, 0);
	if (!vars->w && vars->s1)
		vars->w = ft_strdup(vars->s1);
	else if (vars->s1)
		vars->w = join_free(vars->w, vars->s1);
	vars->i += ft_strlen(vars->s1);
}

void	handling_dollar2(t_vars *vars)
{
	t_env	*current;

	current = g_data.envl;
	while (current != NULL)
	{
		if (!ft_strcmp((current)->key, vars->v))
		{
			if (vars->is_dollar)
				vars->start = 1;
			if ((current)->value && ft_strlen((current)->value) > 0)
				vars->ret = ft_strdup((current)->value);
			break ;
		}
		current = (current)->next;
	}
	current = g_data.envl;
}

void	handling_dollar4(t_vars *vars)
{
	if (!vars->w && vars->ret)
		vars->w = ft_strdup(vars->ret);
	else if (vars->ret)
		vars->w = join_free(vars->w, vars->ret);
	vars->i += ft_strlen(vars->s1);
	if (vars->ret)
		free(vars->ret);
	if (vars->v)
		free(vars->v);
	if (vars->s1)
		free(vars->s1);
}

void	free_struct(t_vars *vars, char c)
{
	if (vars->s1)
		free(vars->s1);
	if (vars->ret)
		free(vars->ret);
	if (vars->v)
		free(vars->v);
	if (c != 'w' && vars->w)
		free(vars->w);
}

char	*handling_dollar5(char *s, t_vars *vars)
{
	if (vars->w)
	{
		// free_struct(vars, 'w');
		return (vars->w);
	}
	else if (vars->is_dollar)
	{
		// free_struct(vars, '\0');
		return (NULL);
	}
	else
	{
		// free_struct(vars, '\0');
		return (s);
	}
}

void	handling_dollar7(char *s, t_vars *vars)
{
	vars->end = equal_export(s, vars->i);
	vars->start = ++(vars->i);
	vars->v = ft_substr(s, vars->start, vars->i - vars->start);
	if (vars->end)
		vars->v = ft_substr(s, vars->start, vars->end - vars->start);
	vars->s1 = str_export_split(s, vars->start, 1);
}

void	init_data(t_vars *vars)
{
	vars->i = 0;
	vars->end = 0;
	vars->start = 0;
	vars->s1 = NULL;
	vars->ret = NULL;
	vars->is_dollar = 0;
	vars->v = NULL;
	vars->w = NULL;
}

char	*handling_dollar(char *s)
{
	t_vars	vars;

	init_data(&vars);
	while (s[vars.i] && check_dollar(s))
	{
		if (ft_strlen(s) == 1 && s[0] == '$')
			return (ft_strdup("$"));
		if (s[vars.i] == '$')
		{
			vars.is_dollar = 1;
			handling_dollar7(s, &vars);
			handling_dollar2(&vars);
			handling_dollar4(&vars);
		}
		else
			handling_dollar3(s, &vars);
	}
	return (handling_dollar5(s, &vars));
}

char	*str_upper(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		s[i] = ft_toupper(s[i]);
		i++;
	}
	return (s);
}

char	*str_lower(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		s[i] = ft_tolower(s[i]);
		i++;
	}
	return (s);
}

void	add_to_args2(int i, int *j, t_sep *node, char **str)
{
	*str = NULL;
	if (i > 0)
	{
		if (node->is_path_in_arg)
			*j = i;
		else
			*j = i - 1;
		node->args = ft_realloc_2(node->args, *j, (*j + 1));
		node->args[*j] = NULL;
	}
}

void	add_to_args3(t_sep *node, char *str, int *i)
{
	handling_builtins(node, str, 0);
	if (!node->is_builtin && node->path)
	{
		node->args = ft_realloc_2(node->args, *i, (*i + 1));
		node->args[*i] = ft_strdup(node->path);
		node->args[*i + 1] = NULL;
		(*i)++;
		node->is_path_in_arg = 1;
		return ;
	}
	(*i)++;
}

void	add_to_args4(char *s, int *l, char **str, int start)
{
	if (s[start] != '\'' && s[start] != '\"')
	{
		*l = start;
		while (s[*l] && s[*l] != ' ' && s[*l] != '\'' && s[*l] != '\"' )
			(*l)++;
		*str = ft_substr(s, start, *l - start);
	}
	else
	{				
		*l = search_sq(s, start + 1, s[start]);
		if (!*l)
			error_msg("error multiligne");
		*str = ft_substr(s, start + 1, *l - start - 1);
	}
	if (s[start] != '\'')
		*str = handling_dollar(*str);
}

void	add_to_args5(t_sep *node, char **str, int j, int *i)
{
	if (!node->args[j] && *str)
	{
		node->args[j] = ft_strdup(*str);
		(*i)++;
	}
	else if (*str)
	{
		node->args[j] = join_free(node->args[j], *str);
		(*i)++;
	}
	if (*str)
		free(*str);
	*str = NULL;
}

void	add_to_args6(char *s, int *start, int l)
{
	if (s[*start] == '\'' || s[*start] == '\"')
		*start = l + 1;
	else
		*start = l;
}

void	add_to_args7(int *tmp, int *start, int *end, char *s)
{
	*tmp = 0;
	*start = *end;
	get_args2(s, end, tmp);
}

int	add_to_args8(t_sep *node, int j, int tmp)
{
	if (node->args[j])
		node->args[j + 1] = NULL;
	return (tmp);
}

int	add_to_args(int *end, char *s, int *i, t_sep *node)
{
	int		l;
	char	*str;
	int		j;
	int		start;
	int		tmp;

	l = 0;
	add_to_args7(&tmp, &start, end, s);
	add_to_args2(*i, &j, node, &str);
	while (s[start] && start < *end)
	{
		add_to_args4(s, &l, &str, start);
		if (*i == 0)
		{
			add_to_args3(node, str, i);
			return (tmp);
		}
		else
		{
			add_to_args5(node, &str, j, i);
			add_to_args6(s, &start, l);
		}
	}
	return (add_to_args8(node, j, tmp));
}

void	handling_builtins(t_sep *node, char *s, int start)
{
	int		l;
	char	*str;

	l = 0;
	str = NULL;
	if (s[start] != '\'' && s[start] != '\"')
	{
		l = start;
		while (s[l] && s[l] != '\'' && s[l] != '\"' && s[l] != ' ')
			l++;
		str = ft_substr(s, start, l - start);
	}
	else
	{				
		l = search_sq(s, start + 1, s[start]);
		if (!l)
			error_msg("error multiligne");
		str = ft_substr(s, start + 1, l - start - 1);
	}
	node->builtin = ft_strdup(str);
	node->upper_builtin = ft_strdup(str_upper(str));
	node->lower_builtin = ft_strdup(str_lower(str));
	check_fill_path(node);
	check_builtin(node);
}

void	get_args2(char *s, int *start, int *end)
{
	while (s[*start] && s[*start] != ' ')
	{
		if (s[*start] == '\'' || s[*start] == '\"')
		{
			*end = search_sq(s, *start + 1, s[*start]);
			if (!*end)
				error_msg("error multiligne");
			*start = *end + 1;
			return ;
		}
		else
			(*start)++;
	}
}

void	check_first_cmd_dollar(char **s, int *start)
{
	int		l;
	char	*str;
	char	*tmp;

	tmp = NULL;
	if (*s[*start] != '\'' && *s[*start] != '\"')
	{
		l = *start;
		while ((*s)[l] && (*s)[l] != ' ' && (*s)[l] != '\'' && (*s)[l] != '\"')
			l++;
		str = ft_substr(*s, *start, l - *start);
	}
	else
	{				
		l = search_sq(*s, *start + 1, *s[*start]);
		if (!l)
			error_msg("error multiligne");
		str = ft_substr(*s, *start + 1, l - *start - 1);
	}
	if (*s[*start] != '\'')
	{
		tmp = handling_dollar(str);
		if (ft_strcmp(tmp, str))
			*s = ft_strjoin(tmp, ft_substr(*s, l - 1, ft_strlen(*s) - l));
	}
}

void	get_args(char *s, int start, t_sep *node)
{
	int		i;
	int		end;
	int		t;

	i = 0;
	t = 0;
	end = 0;
	check_first_cmd_dollar(&s, &start);
	while (s && start < (int)ft_strlen(s))
	{
		end = add_to_args(&start, s, &i, node);
		while (start < (int)ft_strlen(s) && s[start] == ' ')
			start++;
	}
}

void	red_add_to_args(int start, int end, int *i, t_sep *node)
{
	int		l;
	char	*str;
	char	*s;
	int		j;

	l = 0;
	s = node->red_args;
	add_to_args2(*i, &j, node, &str);
	while (s[start] && start < end)
	{
		add_to_args4(s, &l, &str, start);
		if (*i == 0)
			return (add_to_args3(node, str, i));
		else
		{
			add_to_args5(node, &str, j, i);
			if (s[start] == '\'' || s[start] == '\"')
				start = l + 1;
			else
				start = l;
		}
	}
	if (node->args[j])
		node->args[j + 1] = NULL;
}

char	*red_redim_s(char *s, int start, int end)
{
	char	*s1;
	char	*s2;

	s1 = ft_substr(s, 0, start);
	s2 = ft_substr(s, end + 1, ft_strlen(s) - end + 1);
	s = ft_strjoin(s1, " ");
	s = join_free(s, s2);
	free(s1);
	free(s2);
	return (s);
}

char	*red_get_file2(t_sep *node, int i, int start, char *s)
{
	char	*file;
	int		end;

	file = NULL;
	end = search_sq(s, start + 1, s[start]);
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
	return (file);
}

char	*red_get_file3(t_sep *node, int i, int start, char *s)
{
	char	*file;
	int		end;

	file = NULL;
	end = start;
	while (s[end] && s[end] != ' ' && s[end] != '<' && s[end] != '>')
		(end)++;
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
	return (file);
}

char	*red_get_file(t_sep *node, char *s, int start, char type)
{
	int	i;

	i = 0;
	while (s[start])
	{
		node->red_args = NULL;
		i = start;
		if (type == 'i' || type == 'o' )
			start++;
		else if (type == 'a' || type == 'h')
			start += 2;
		while (s[start] && s[start] == ' ')
			start++;
		if (s[start] == '\'' || s[start] == '\"')
		{
			if (i != 0 && s[start - 1] == '\\')
				continue ;
			return (red_get_file2(node, i, start, s));
		}
		else
			return (red_get_file3(node, i, start, s));
	}
	return (NULL);
}

void	red_get_cmd_args2(t_sep *node, int *start, int *end, int *i)
{
	*end = *start;
	while (node->red_args[*end] && node->red_args[*end] != ' ')
		(*end)++;
	if (*end > *start)
		red_add_to_args(*start, *end, i, node);
	*start = *end;
}

void	red_get_cmd_args(t_sep *node)
{
	int		i;
	int		end;
	int		start;

	i = 0;
	start = 0;
	while (node->red_args[start])
	{
		if (node->red_args[start] == '\'' || node->red_args[start] == '\"')
		{
			if (node->red_args[start - 1] == '\\')
				continue ;
			end = search_sq(node->red_args, start + 1, node->red_args[start]);
			if (!end)
				error_msg("error multiligne");
			red_add_to_args(start, end, &i, node);
			start = end + 1;
		}
		else
			red_get_cmd_args2(node, &start, &end, &i);
		while (node->red_args[start] && node->red_args[start] == ' ')
			start++;
	}
}

char	red_get_type(char *s, int start)
{
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
				if (s[start + 2] == '<')
					error_msg("syntax error near unexpected token `<'");
				return ('h');
			}
			return ('i');
		}
		start++;
	}
	return ('\0');
}

void	init_node_red(t_red *red)
{
	red->r_file = NULL;
	red->red_op = '\0';
}

void	fill_node_red(t_red *red, char *file, char type)
{
	red->r_file = file;
	red->red_op = type;
}

void	fill_red_list(t_red **head, char *file, char type)
{
	int		i;
	t_red	*newnode;
	t_red	*lastnode;

	i = 0;
	newnode = malloc(sizeof(t_red));
	lastnode = *head;
	init_node_red(newnode);
	fill_node_red(newnode, file, type);
	newnode->next = NULL;
	if (*head == NULL)
		*head = newnode;
	else
	{
		lastnode = *head;
		while (lastnode->next != NULL)
		{
			lastnode = lastnode->next;
		}
		lastnode->next = newnode;
	}
}

void	red_get_type_file2(int *i, int *end, char *s)
{
	while (s[*i] && s[*i] == ' ')
		(*i)++;
	if (s[*i] == '\'' || s[*i] == '\"')
	{
		*end = search_sq(s, *i + 1, s[*i]);
		if (!*end)
			error_msg("error multiligne");
		*i = *end;
	}
}

void	red_get_type_file(t_sep *node, char *s)
{
	int		i;
	char	type;
	char	*file;
	int		end;

	i = 0;
	file = NULL;
	end = 0;
	node->red_args = NULL;
	while (s[i])
	{
		red_get_type_file2(&i, &end, s);
		if ((s[i] == '>' ) || (s[i] == '<' ))
		{
			type = red_get_type(s, i);
			file = red_get_file(node, s, i, type);
			s = node->red_args;
			fill_red_list(&node->red, file, type);
		}
		if ((int)ft_strlen(s) < i + 1)
			break ;
		i++;
	}
	red_get_cmd_args(node);
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
	node->s_red = NULL;
	node->r_args = NULL;
	node->is_path_in_arg = 0;
	node->red = NULL;
}

int	check_red(t_sep *node, char *s)
{
	int	start;
	int	end;

	start = 0;
	while (s[start])
	{
		if (s[start] == '\'' || s[start] == '\"')
		{
			end = search_sq(s, start + 1, s[start]);
			if (!end)
				error_msg("error multiligne");
			start = end;
		}
		else if ((start == 0 && (s[start] == '>' || s[start] == '<'))
			|| (start > 0 && (s[start] == '>' || s[start] == '<')
				&& s[start - 1] != '\\'))
		{
			node->is_red = 1;
			red_get_type_file(node, s);
			return (1);
		}
		start++;
	}
	return (0);
}

void	get_builtin(char *s, t_sep *node)
{
	int		i;
	int		l;

	i = 0;
	l = ft_strlen(s);
	if (s[i] == '|')
	{
		i++;
		while (s[i] && s[i] == ' ')
			i++;
		s = ft_substr(s, i, l - i);
	}
	i = 0;
	while (s[i] && s[i] == ' ')
		i++;
	if (!check_red(node, s))
		get_args(s, i, node);
}

void	parsing_red(t_sep *node, char *s)
{
	int	i;
	int	end;

	i = 0;
	end = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '\"')
		{
			if (i != 0 && s[i - 1] == '\\')
				continue ;
			end = search_sq(s, i + 1, s[i]);
			if (!end)
				error_msg("error multiligne");
			i = end;
		}
		else
		{
			if ((i == 0 && (s[i] == '>' || s[i] == '<'))
				|| (i > 0 && (s[i] == '>' || s[i] == '<') && s[i - 1] != '\\'))
				node->is_red = 1;
		}
		i++;
	}
}

void	fill_node(char *s, t_sep *node, int start, char *str)
{
	int	i;
	int	l;

	i = 0;
	l = ft_strlen(s);
	if (str[start + l] && (str[start + l] == '|' || str[start + l] == ';'))
		node->t_sp = str[start + l];
	node->s_red = ft_strdup(s);
	get_builtin(s, node);
}

void	addlast_sep(t_sep **head, char *s, int start, char *str)
{
	int		i;
	t_sep	*newnode;
	t_sep	*lastnode;

	i = 0;
	newnode = malloc(sizeof(t_sep));
	lastnode = *head;
	init_t_sep(newnode);
	fill_node(s, newnode, start, str);
	newnode->next = NULL;
	if (*head == NULL)
		*head = newnode;
	else
	{
		lastnode = *head;
		while (lastnode->next != NULL)
		{
			lastnode = lastnode->next;
		}
		lastnode->next = newnode;
	}
}

void	free_mylist_red(t_red *head)
{
	t_red	*tmp;

	while (head != NULL)
	{
		tmp = head;
		free(tmp->r_file);
		head = head->next;
		free(tmp);
	}
}

void	free_mylist_sep(t_sep *head)
{
	t_sep	*tmp;
	int		i;
	int		l;

	i = 0;
	l = 0;
	while (head != NULL)
	{
		tmp = head;
		free(tmp->path);
		free(tmp->builtin);
		free(tmp->upper_builtin);
		free(tmp->red_args);
		free(tmp->s_red);
		l = ft_strlen2(tmp->args);
		while (l > i)
			free(tmp->args[i++]);
		head = head->next;
		free(tmp);
	}
}

void	free_mylist_env(t_data *data)
{
	t_env	*tmp;
	t_env	*env;
	int		i;

	i = 0;
	env = data->envl;
	while (env != NULL)
	{
		tmp = env;
		free(tmp->val);
		free(tmp->value);
		free(tmp->key);
		env = env->next;
		free(tmp);
	}
	while (data->envp[i])
	{
		free(data->envp[i]);
		i++;
	}
}

void	fill_list3(t_sep **head, char *str, int i, int *start)
{
	char	*s;

	while (str[*start] && str[*start] == ' ')
		(*start)++;
	if (!(str)[i + 1])
		s = ft_substr(str, *start, i - *start + 1);
	else
		s = ft_substr(str, *start, i - *start);
	if (ft_strlen(s) > 0)
		addlast_sep(head, s, *start, str);
	if (s)
		free(s);
	*start = i;
}

void	fill_list2(char *str, int *pipes_num, t_sep **head)
{
	int	i;
	int	start;
	int	end;

	i = 0;
	start = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			end = search_sq(str, i + 1, str[i]);
			if (!end)
				error_msg("error multiligne");
			i = end;
		}
		if ((str[i] == '|') || (!str[i + 1]))
		{
			*pipes_num += (str[i] == '|');
			if (i && str[i - 1] == '\\')
				error_msg("error multiligne");
			fill_list3(head, str, i, &start);
		}
		i++;
	}
}

int	fill_list(char *str)
{
	int		pipes_num;
	t_sep	*head;
	int		exit_status;

	head = NULL;
	pipes_num = 0;
	fill_list2(str, &pipes_num, &head);
	// print_mylist(head, pipes_num);
	exit_status = run_cmdline(head, pipes_num);
	set_exit_code(exit_status);
	// if (head->is_red)
	// 	free_mylist_red(head->red);
	// free_mylist_sep(head);
	return (0);
}

int	fill_args(char *str)
{
	int	i;

	i = 0;
	return (fill_list(str));
}

void	fill_env2(t_env	**temp, char **env, int i)
{
	char	**s;


	s = ft_split(env[i], '=');
	*temp = malloc(sizeof(**temp));
	(*temp)->val = ft_strdup(env[i]);
	(*temp)->key = ft_strdup(s[0]);
	(*temp)->value = ft_strdup(s[1]);
	(*temp)->next = NULL;
	free_t2(s);
}

void	fill_env3(t_env **temp)
{
	*temp = malloc(sizeof(**temp));
	(*temp)->val = ft_strdup("?=0");
	(*temp)->key = ft_strdup("?");
	(*temp)->value = ft_strdup("0");
	(*temp)->next = NULL;
}

t_env	*fill_env(char **env)
{
	t_env	*head;
	t_env	*last;
	t_env	*temp;
	int		i;

	i = 0;
	head = NULL;
	while (env[i])
	{
		fill_env2(&temp, env, i);
		if (!head)
		{
			head = temp;
			last = temp;
		}
		else
		{
			last->next = temp;
			last = last->next;
		}
		i++;
	}
	fill_env3(&temp);
	last->next = temp;
	return (head);
}

char	*my_getcwd(void)
{
	t_env	*current;

	current = g_data.envl;
	while (current != NULL)
	{
		if (ft_strcmp(current->key, "PWD") == 0)
		{
			return (ft_strjoin(current->value, "> "));
			break ;
		}
		current = current->next;
	}
	return (NULL);
}

void	ignctl(void)
{
	struct termios	term;

	if (tcgetattr(STDIN_FILENO, &term) != 0)
		perror("tcgetattr() error");
	term.c_lflag &= ~ECHOCTL;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &term) != 0)
		perror("tcsetattr() error");
}

int		main(int argc, char **argv, char **env)
{
	int i;
	char *str;
	int ret;
	
	(void)argc;
	(void)argv;
	
	g_data.envp = env;
	g_data.is_forked = 0;
	g_data.envl = fill_env(env);
	i = 0;
	ret = 0;
	ignctl();
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, signal_handler_parent);
	while (1)
	{
		str = NULL;
		str = readline("\x1B[32m$> \e[0m");
		if (!str)
		{
			//if (isatty(0))
				write(2, "\b\bexit\n", 7); // return 1
			break ;
		}
		if (str[0] == '\0')
		{
			free(str);
			continue ;
		}
		add_history(str);
		if (!handling_errors_arg(str))
		{
			if (fill_args(str) == 1)
			{
				free(str);
				return (1);
			}
		}
		free(str);
		// system("leaks minishell");
	}
	// free_mylist_env(&g_data);
	return (0);
}