/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 11:08:48 by mhaddi            #+#    #+#             */
/*   Updated: 2021/11/16 11:37:44 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	run_child(t_sep *node)
{
	g_data.is_forked = 1;
	if (execve((char *[2]){node->path, node->builtin}[!node->path],
				node->args, g_data.envp) == -1)
	{
		printf("minishell: %s: command not found\n", node->builtin);
		exit(127);
	}
}

int	run_executable(t_sep *node)
{
	int		exit_status;
	pid_t	fork_pid;

	exit_status = 0;
	fork_pid = fork();
	if (check_error(fork_pid < 0, NULL, "minishell: fork", -1))
		return (1);
	if (fork_pid == 0)
		run_child(node);
	else
	{
		if (check_error(signal(SIGINT, SIG_IGN) == SIG_ERR,
				NULL, "minishell: signal", -1))
			return (1);
		if (check_error(waitpid(fork_pid, &exit_status, 0) < 0,
				NULL, "minishell: waitpid", -1))
			return (1);
		exit_status = WEXITSTATUS(exit_status);
		if (check_error(signal(SIGINT, signal_handler_parent) == SIG_ERR,
				NULL, "minishell: signal", -1))
			return (1);
	}
	return (exit_status);
}

int	run_cmd(t_sep *node, int *stdin_fd, int *stdout_fd)
{
	int	exit_status;

	exit_status = 0;
	if (node->is_builtin)
		exit_status = run_builtins(node, 0);
	else if (node->path || node->builtin)
		exit_status = run_executable(node);
	if (dup2(*stdin_fd, 0) < 0)
	{
		printf("minishell: dup2: %s\n", strerror(errno));
		return (1);
	}
	if (dup2(*stdout_fd, 1) < 0)
	{
		printf("minishell: dup2: %s\n", strerror(errno));
		return (1);
	}
	return (exit_status);
}
