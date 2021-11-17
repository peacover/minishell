/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_dollar2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <yer-raki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 08:02:53 by yer-raki          #+#    #+#             */
/*   Updated: 2021/11/17 08:13:20 by yer-raki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

int	equal_export(char *s, int i)
{
	i++;
	while (s[i] && s[i] != '$' && (ft_isalpha(s[i]) || s[i] == '_'
			|| ft_isdigit(s[i]) || (s[i] == '?' && s[i - 1] == '$')))
		i++;
	return (i);
}

void	handling_dollar7(char *s, t_vars *vars)
{
	vars->end = equal_export(s, vars->i);
	vars->start = ++(vars->i);
	vars->v = ft_substr(s, vars->start, vars->i - vars->start);
	add_garbage((void *)&(vars->v));
	if (vars->end)
		vars->v = ft_substr(s, vars->start, vars->end - vars->start);
	add_garbage((void *)&(vars->v));
	vars->s1 = str_export_split(s, vars->start, 1);
	add_garbage((void *)&(vars->s1));
}

void	check_first_cmd_dollar2(char *s, int start, int *l, char **str)
{
	if (s[start] != '\'' && s[start] != '\"')
	{
		*l = start;
		while ((s)[*l] && (s)[*l] != ' ' && (s)[*l] != '\'' && (s)[*l] != '\"')
			(*l)++;
		*str = ft_substr(s, start, *l - start);
		add_garbage((void *)&*str);
	}
	else
	{				
		*l = search_sq(s, start + 1, s[start]);
		if (!*l)
			error_msg("error multiligne");
		*str = ft_substr(s, start + 1, *l - start - 1);
		add_garbage((void *)str);
	}
}

void	check_first_cmd_dollar(char **s, int *start)
{
	int		l;
	char	*str;
	char	*tmp;

	tmp = NULL;
	check_first_cmd_dollar2(*s, *start, &l, &str);
	if (*s[*start] != '\'')
	{
		tmp = handling_dollar(str);
		if (ft_strcmp(tmp, str))
		{
			*s = ft_strjoin(tmp, ft_substr(*s, l - 1, ft_strlen(*s) - l));
			add_garbage((void *)s);
		}
	}
}
