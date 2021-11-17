/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_args2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <yer-raki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 07:25:22 by yer-raki          #+#    #+#             */
/*   Updated: 2021/11/17 08:19:53 by yer-raki         ###   ########.fr       */
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
