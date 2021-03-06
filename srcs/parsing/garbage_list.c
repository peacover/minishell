/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_list.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <yer-raki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 12:17:03 by yer-raki          #+#    #+#             */
/*   Updated: 2021/11/17 09:01:28 by yer-raki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_garbage(void)
{
	t_garbage	*tmp;

	while (!g_data.gg)
	{
		if (g_data.gg->str)
			free(g_data.gg->str);
		tmp = g_data.gg;
		g_data.gg = g_data.gg->next;
		free(tmp);
	}	
}

void	add_garbage(void **str)
{
	t_garbage	*newnode;

	newnode = (void *)malloc(sizeof(t_garbage));
	newnode->str = *str;
	newnode->next = g_data.gg;
	g_data.gg = newnode;
}
