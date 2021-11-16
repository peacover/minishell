/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_builtins.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 11:12:42 by mhaddi            #+#    #+#             */
/*   Updated: 2021/11/16 16:01:57 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	run_builtins(t_sep *node, int is_in_pipe)
{
	int		exit_status;

	exit_status = 0;
	if (ft_strcmp(node->lower_builtin, "echo") == 0)
		exit_status = echo(node->args);
	if (ft_strcmp(node->lower_builtin, "cd") == 0)
		exit_status = cd(node->args);
	if (ft_strcmp(node->lower_builtin, "pwd") == 0)
		pwd();
	if (ft_strcmp(node->lower_builtin, "export") == 0)
		exit_status = ft_export(node->args);
	if (ft_strcmp(node->lower_builtin, "unset") == 0)
		exit_status = unset(node->args);
	if (ft_strcmp(node->lower_builtin, "env") == 0)
		env();
	if (ft_strcmp(node->lower_builtin, "exit") == 0)
		exit_status = ft_exit(node->args, is_in_pipe);
	if (is_in_pipe)
		exit(ft_atoi(get_last_exit_code()));
	return (exit_status);
}
