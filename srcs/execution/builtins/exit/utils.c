/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 16:36:07 by mhaddi            #+#    #+#             */
/*   Updated: 2021/11/16 16:36:15 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

void	set_exit_code(int value)
{
	t_env	*current;

	current = g_data.envl;
	while (current != NULL)
	{
		if (ft_strcmp(current->key, "?") == 0)
		{
			free(current->value);
			current->value = ft_itoa(value);
			break ;
		}
		current = current->next;
	}
}

char	*get_last_exit_code(void)
{
	t_env	*current;

	current = g_data.envl;
	while (current != NULL)
	{
		if (ft_strcmp(current->key, "?") == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}
