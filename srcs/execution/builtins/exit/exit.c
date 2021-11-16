/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 16:01:09 by mhaddi            #+#    #+#             */
/*   Updated: 2021/11/16 16:01:23 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

void	exit_when_arg_is_non_numeric(int is_in_pipe, char *arg)
{
	set_exit_code(255);
	if (!is_in_pipe)
		printf("exit\n");
	printf("minishell: exit: %s: numeric argument required\n", arg);
	exit(255);
}

int	exit_when_multiple_args(int is_in_pipe, char *arg)
{
	if (is_number(arg))
	{
		set_exit_code(1);
		if (!is_in_pipe)
			printf("exit\n");
		printf("minishell: exit: too many arguments\n");
		return (1);
	}
	else
		exit_when_arg_is_non_numeric(is_in_pipe, arg);
	return (0);
}

void	exit_when_single_arg(int is_in_pipe, char *arg)
{
	int	number;

	if (is_number(arg))
	{
		number = ft_atoi(arg);
		while (number > 256)
			number -= 256;
		while (number < 0)
			number += 256;
		set_exit_code(number);
		if (!is_in_pipe)
			printf("exit\n");
		exit(number);
	}
	else
		exit_when_arg_is_non_numeric(is_in_pipe, arg);
}

int	ft_exit(char **args, int is_in_pipe)
{
	int	args_num;

	args_num = 0;
	if (args)
		while (args[args_num])
			args_num++;
	if (args_num > 1)
		if (exit_when_multiple_args(is_in_pipe, args[0]))
			return (1);
	if (args_num == 1)
		exit_when_single_arg(is_in_pipe, args[0]);
	else
	{
		if (!is_in_pipe)
			printf("exit\n");
		exit(ft_atoi(get_last_exit_code()));
	}
	return (0);
}
