/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <yer-raki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 12:06:05 by yer-raki          #+#    #+#             */
/*   Updated: 2021/11/17 11:40:17 by yer-raki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	add_garbage((void *)&newnode);
	lastnode = *head;
	init_t_sep(newnode);
	fill_node(s, newnode, start, str);
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

void	fill_list3(t_sep **head, char *str, int i, int *start)
{
	char	*s;

	while (str[*start] && str[*start] == ' ')
		(*start)++;
	if (!(str)[i + 1])
		s = ft_substr(str, *start, i - *start + 1);
	else
		s = ft_substr(str, *start, i - *start);
	add_garbage((void *)&s);
	if (ft_strlen(s) > 0)
		addlast_sep(head, s, *start, str);
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
			fill_list3(head, str, i, &start);
		}
		i++;
	}
}

void	fill_list(char *str)
{
	int		pipes_num;
	t_sep	*head;
	int		exit_status;

	head = NULL;
	pipes_num = 0;
	fill_list2(str, &pipes_num, &head);
	print_mylist(head,pipes_num);
	exit_status = run_cmdline(head, pipes_num);
	set_exit_code(exit_status);
}
