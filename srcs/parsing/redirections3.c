/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <yer-raki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 07:30:32 by yer-raki          #+#    #+#             */
/*   Updated: 2021/11/17 11:44:28 by yer-raki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*red_get_file2(t_sep *node, int i, int start, char *s)
{
	char	*file;
	int		end;

	file = NULL;
	end = search_sq(s, start + 1, s[start]);
	if (!end)
		error_msg("error multiligne");
	file = ft_substr(s, start + 1, end - start - 1);
	add_garbage((void *)&file);
	if (s[start] != '\'')
		file = handling_dollar(file);
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
	add_garbage((void *)&file);
	node->red_args = red_redim_s(s, i, end - 1);
	if (s[start] != '\'')
		file = handling_dollar(file);
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
			return (red_get_file2(node, i, start, s));
		else
			return (red_get_file3(node, i, start, s));
	}
	return (NULL);
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
