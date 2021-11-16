/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 15:59:49 by mhaddi            #+#    #+#             */
/*   Updated: 2021/11/16 16:35:07 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

void	env(void)
{
	t_env	*current;

	current = g_data.envl;
	while (current != NULL)
	{
		if (current->value && current->key[0] != '?')
			printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
}
