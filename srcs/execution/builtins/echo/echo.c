/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 15:21:56 by mhaddi            #+#    #+#             */
/*   Updated: 2021/11/16 16:34:39 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

int	echo(char **args)
{
	int	no_newline;
	int	i;

	if (!args)
	{
		printf("\n");
		return (0);
	}
	no_newline = 0;
	if (ft_strcmp(args[0], "-n") == 0)
		no_newline = 1;
	i = no_newline;
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (no_newline == 0)
		printf("\n");
	return (0);
}
