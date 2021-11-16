/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 11:27:37 by mhaddi            #+#    #+#             */
/*   Updated: 2021/11/16 11:40:49 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	run_parent_process(pid_t *pids, int *num_cmd,
		int *exit_status, int pipe_fd[2])
{
	if (check_error(
			signal(SIGINT, SIG_IGN) == SIG_ERR, pids, "minishell: signal", -1))
		return (1);
	if (check_error(waitpid(pids[*num_cmd], exit_status, 0) < 0,
			pids, "minishell: waitpid", -1))
		return (1);
	*exit_status = WEXITSTATUS(*exit_status);
	if (check_error(signal(SIGINT, signal_handler_parent) == SIG_ERR,
			pids, "minishell: signal", -1))
		return (1);
	close(pipe_fd[1]);
	(*num_cmd)++;
	return (0);
}
