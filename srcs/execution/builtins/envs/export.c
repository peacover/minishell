/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 15:40:26 by mhaddi            #+#    #+#             */
/*   Updated: 2021/11/16 15:56:53 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

int	do_export_with_args(char **args)
{
	int		i;
	int		exit_status;

	i = 0;
	exit_status = 0;
	while (args[i])
	{
		exit_status = ft_putenv(args[i]);
		i++;
	}
	return (exit_status);
}

void	do_export_with_noargs(void)
{
	t_env	*current;

	current = g_data.envl;
	while (current != NULL)
	{
		if (current->key[0] != '?')
		{
			printf("declare -x %s", current->key);
			if (current->value)
				printf("=\"%s\"", current->value);
			printf("\n");
		}
		current = current->next;
	}
}

int	ft_export(char **args)
{
	int		exit_status;

	exit_status = 0;
	if (args && *args)
		exit_status = do_export_with_args(args);
	else
		do_export_with_noargs();
	return (exit_status);
}
