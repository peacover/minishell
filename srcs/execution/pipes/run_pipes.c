/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_pipes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 10:56:17 by mhaddi            #+#    #+#             */
/*   Updated: 2021/11/16 11:40:34 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	run_pipe_executable(t_sep *node, int pipe_fd[2])
{
	if (execve((char *[2]){node->path, node->builtin}[!node->path],
				node->args,
				g_data.envp) == -1)
	{
		printf("minishell: %s: command not found\n", node->builtin);
		close(pipe_fd[1]);
		exit(127);
	}
}

void	run_piped_process(int *ptrs[3], t_sep *node,
		int pipe_fd[2], pid_t *pids)
{
	int	*num_cmd;
	int	*pipes_num;
	int	*exit_status;

	num_cmd = ptrs[0];
	pipes_num = ptrs[1];
	exit_status = ptrs[2];
	g_data.is_forked = 1;
	if (*num_cmd < *pipes_num && (node->path || node->builtin))
		check_error(dup2(pipe_fd[1], 1) < 0, pids, "minishell: dup2", 1);
	if (node->is_red)
		if (pipe_redirect(node, pids) == 1)
			exit(1);
	if (node->is_builtin)
		*exit_status = run_builtins(node, 1);
	else if (node->path || node->builtin)
		run_pipe_executable(node, pipe_fd);
}

int	create_pipe(int *stdin_fd, int pipe_fd[2], pid_t *pids)
{
	*stdin_fd = dup(0);
	if (check_error(*stdin_fd < 0 || dup2(pipe_fd[0], 0) < 0,
			pids, "minishell: dup", -1))
		return (1);
	if (check_error(pipe(pipe_fd) < 0, pids, "minishell: pipe", -1))
		return (1);
	return (0);
}

int	run_piped_cmd(int *ptrs[4], int pipe_fd[2], pid_t *pids, t_sep *node)
{
	int	*num_cmd;
	int	*pipes_num;
	int	*exit_status;
	int	*stdin_fd;

	num_cmd = ptrs[0];
	pipes_num = ptrs[1];
	exit_status = ptrs[2];
	stdin_fd = ptrs[3];
	if (*num_cmd > 0)
		create_pipe(stdin_fd, pipe_fd, pids);
	pids[*num_cmd] = fork();
	check_error(pids[*num_cmd] < 0, pids, "minishell: fork", 1);
	if (pids[*num_cmd] == 0)
		run_piped_process(
			(int *[3]){num_cmd, pipes_num, exit_status}, node, pipe_fd, pids);
	else
	{
		if (run_parent_process(pids, num_cmd, exit_status, pipe_fd))
			return (1);
	}
	return (0);
}

int	run_pipes(t_sep *node, int pipes_num, int *stdin_fd, int *stdout_fd)
{
	int		exit_status;
	pid_t	*pids;
	int		pipe_fd[2];
	int		num_cmd;

	exit_status = 0;
	pids = malloc(sizeof(pid_t) * (pipes_num + 1));
	if (check_error(pipe(pipe_fd) < 0, pids, "minishell: pipe", -1))
		return (1);
	num_cmd = 0;
	while (node != NULL)
	{
		if (node->t_sp == '|' || num_cmd == pipes_num)
			if (run_piped_cmd((int *[4]){&num_cmd, &pipes_num,
					&exit_status, stdin_fd}, pipe_fd, pids, node))
				return (1);
		check_error(dup2(*stdin_fd, 0) < 0 || dup2(*stdout_fd, 1) < 0,
			pids, "minishell: dup2", 1);
		node = node->next;
	}
	return (exit_status);
}
