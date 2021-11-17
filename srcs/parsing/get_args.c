/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_args.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <yer-raki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 12:28:16 by yer-raki          #+#    #+#             */
/*   Updated: 2021/11/17 08:01:12 by yer-raki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
