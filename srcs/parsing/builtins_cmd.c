/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <yer-raki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 12:24:55 by yer-raki          #+#    #+#             */
/*   Updated: 2021/11/17 10:26:33 by yer-raki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_fill_path2(char *w, t_sep *node)
{
	int		fd;
	char	*s;

	fd = 0;
	s = ft_strjoin(w, "/");
	add_garbage((void *)&s);
	s = ft_strjoin(s, node->lower_builtin);
	add_garbage((void *)&s);
	if (node->builtin[0] && node->builtin[0] == '/')
		fd = open(node->builtin, O_RDONLY);
	else
		fd = open(s, O_RDONLY);
	if (fd > 0)
	{
		if (node->builtin[0] && node->builtin[0] == '/')
			node->path = node->builtin;
		else
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
			while (w && w[i])
			{
				if (check_fill_path2(w[i], node))
					return (free_t2(w));
				i++;
			}
			if (w)
				free_t2(w);
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

void	fill_builtin(t_sep *node, char *str)
{
	if (str)
		node->builtin = ft_strdup(str);
	node->upper_builtin = ft_strdup(str_upper(str));
	node->lower_builtin = ft_strdup(str_lower(str));
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
		add_garbage((void *)&str);
	}
	else
	{		
		l = search_sq(s, start + 1, s[start]);
		if (!l)
			error_msg("error multiligne");
		str = ft_substr(s, start + 1, l - start - 1);
		add_garbage((void *)&str);
	}
	fill_builtin(node, str);
	check_fill_path(node);
	check_builtin(node);
}
