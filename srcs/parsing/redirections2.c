/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <yer-raki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 07:30:27 by yer-raki          #+#    #+#             */
/*   Updated: 2021/11/17 13:26:05 by yer-raki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

void	red_get_cmd_args2(t_sep *node, int *start, int *end, int *i)
{
	*end = *start;
	while (node->red_args[*end] && node->red_args[*end] != ' '
			&& node->red_args[*end] != '\'' && node->red_args[*end] != '\"')
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
