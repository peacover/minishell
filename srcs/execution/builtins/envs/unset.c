/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 15:57:38 by mhaddi            #+#    #+#             */
/*   Updated: 2021/11/16 16:35:00 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

int	unset_key(int i, t_env *current, t_env *prev)
{
	t_env	*tmp;

	tmp = current->next;
	if (i)
		prev->next = tmp;
	else
		g_data.envl = tmp;
	free(current->val);
	free(current->key);
	free(current->value);
	free(current);
	return (0);
}

int	ft_unsetenv(char *key)
{
	int		i;
	t_env	*current;
	t_env	*prev;

	i = 0;
	current = g_data.envl;
	if (check_key_syntax(*key == '\0', key))
		return (1);
	while (key[i])
	{
		if (check_key_syntax(!ft_isalnum(key[i]) && key[i] != '_', key))
			return (1);
		i++;
	}
	i = 0;
	while (current != NULL)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (unset_key(i, current, prev));
		prev = current;
		current = current->next;
		i++;
	}
	return (0);
}

int	unset(char **args)
{
	int		i;
	int		exit_status;

	i = 0;
	exit_status = 0;
	if (args)
	{
		while (args[i])
		{
			if (ft_unsetenv(args[i]) == 1)
				exit_status = 1;
			i++;
		}
	}
	return (exit_status);
}
