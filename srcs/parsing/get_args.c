/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_args.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <yer-raki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 12:28:16 by yer-raki          #+#    #+#             */
/*   Updated: 2021/11/16 18:14:54 by yer-raki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
		add_garbage((void *)str);
	}
	else
	{				
		*l = search_sq(s, start + 1, s[start]);
		if (!*l)
			error_msg("error multiligne");
		*str = ft_substr(s, start + 1, *l - start - 1);
		add_garbage((void *)str);
	}
	if (s[start] != '\'')
		*str = handling_dollar(*str);
}

void	add_to_args5(t_sep *node, char **str, int j, int *i)
{
	if (!node->args[j] && *str)
	{
		node->args[j] = ft_strdup(*str);
		add_garbage((void *)&(node->args[j]));
		(*i)++;
	}
	else if (*str)
	{
		node->args[j] = ft_strjoin(node->args[j], *str);
		add_garbage((void *)&(node->args[j]));
		if (j == *i - 1)
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