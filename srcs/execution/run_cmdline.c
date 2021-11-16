/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmdline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <yer-raki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 15:34:01 by mhaddi            #+#    #+#             */
/*   Updated: 2021/11/16 11:32:10 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	run_cmdline(t_sep *node, int pipes_num)
{
	int		stdin_fd;
	int		stdout_fd;
	int		exit_status;

	stdin_fd = 0;
	stdout_fd = 0;
	exit_status = run_heredoc(node);
	if (exit_status)
		return (exit_status);
	redirect(&stdin_fd, &stdout_fd, node, &exit_status);
	if (node && node->next == NULL && !exit_status)
		exit_status = run_cmd(node, &stdin_fd, &stdout_fd);
	else if (node && node->next)
		exit_status = run_pipes(node, pipes_num, &stdin_fd, &stdout_fd);
	g_data.is_forked = 0;
	return (exit_status);
}
