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

int	check_error(int condition, void *to_free, char *to_print, int exit_code)
{
	if (condition)
	{
		if (to_free)
			free(to_free);
		if (to_print)
			printf("%s: %s\n", to_print, strerror(errno));
		if (exit_code > -1)
			exit(exit_code);
		return (1);
	}
	return (0);
}

void	signal_handler_heredoc(int sig)
{
	if (sig == SIGINT && g_data.is_forked)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		exit(1);
	}
}

void	signal_handler_parent(int sig)
{
	if (sig == SIGINT && g_data.is_forked)
	{
		write(1, "\n", 1);
		rl_on_new_line();
	}
	if (sig == SIGINT && !g_data.is_forked)
	{
		write(1, "\n", 1);
		rl_replace_line("", 1);
		rl_on_new_line();
		rl_redisplay();
		set_exit_code(1);
	}
}

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
