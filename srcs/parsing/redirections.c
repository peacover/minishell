/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <yer-raki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 07:29:56 by yer-raki          #+#    #+#             */
/*   Updated: 2021/11/17 11:47:59 by yer-raki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	add_garbage((void *)&newnode);
	lastnode = *head;
	init_node_red(newnode);
	fill_node_red(newnode, file, type);
	newnode->next = NULL;
	if (*head == NULL)
		*head = newnode;
	else
	{
		lastnode = *head;
		while (lastnode->next)
			lastnode = lastnode->next;
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
			|| (start > 0 && (s[start] == '>' || s[start] == '<')))
		{
			node->is_red = 1;
			red_get_type_file(node, s);
			return (1);
		}
		start++;
	}
	return (0);
}
