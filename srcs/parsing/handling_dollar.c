/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_dollar.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <yer-raki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 12:12:13 by yer-raki          #+#    #+#             */
/*   Updated: 2021/11/17 09:00:11 by yer-raki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	vars->ret = NULL;
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
	{
		free(vars->ret);
		vars->ret = NULL;
	}
	if (vars->v)
	{
		free(vars->v);
		vars->v = NULL;
	}
	if (vars->s1)
	{
		free(vars->s1);
		vars->s1 = NULL;
	}
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
