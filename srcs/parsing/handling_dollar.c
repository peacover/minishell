/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_dollar.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <yer-raki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 12:12:13 by yer-raki          #+#    #+#             */
/*   Updated: 2021/11/16 12:31:36 by yer-raki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	add_garbage((void *)&(vars->s1));
	if (!vars->w && vars->s1)
		vars->w = ft_strdup(vars->s1);
	else if (vars->s1)
		vars->w = ft_strjoin(vars->w, vars->s1);
	add_garbage((void *)&(vars->w));
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
		vars->w = ft_strjoin(vars->w, vars->ret);
	add_garbage((void *)&(vars->w));
	vars->i += ft_strlen(vars->s1);
	if (vars->ret)
		free(vars->ret);
	if (vars->v)
		free(vars->v);
	if (vars->s1)
		free(vars->s1);
}

char	*handling_dollar5(char *s, t_vars *vars)
{
	if (vars->w)
		return (vars->w);
	else if (vars->is_dollar)
		return (NULL);
	else
		return (s);
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