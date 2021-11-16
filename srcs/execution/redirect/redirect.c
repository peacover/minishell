/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 10:54:15 by mhaddi            #+#    #+#             */
/*   Updated: 2021/11/16 11:42:57 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	redirect_stdin(int *stdin_fd, int *input_fd)
{
	*stdin_fd = dup(0);
	if (*stdin_fd < 0)
	{
		printf("minishell: dup: %s\n", strerror(errno));
		return (1);
	}
	if (dup2(*input_fd, 0) < 0)
	{
		printf("minishell: dup2: %s\n", strerror(errno));
		return (1);
	}
	return (0);
}

int	redirect_stdout(int *stdout_fd, int *output_fd)
{
	*stdout_fd = dup(1);
	if (*stdout_fd < 0)
	{
		printf("minishell: dup: %s\n", strerror(errno));
		return (1);
	}
	if (dup2(*output_fd, 1) < 0)
	{
		printf("minishell: dup2: %s\n", strerror(errno));
		return (1);
	}
	return (0);
}

void	redirect_fds(int *fds[4], int is_input, int is_output, int *exit_status)
{
	int	*stdin_fd;
	int	*input_fd;
	int	*stdout_fd;
	int	*output_fd;

	stdin_fd = fds[0];
	input_fd = fds[1];
	stdout_fd = fds[2];
	output_fd = fds[3];
	if (is_input && !*exit_status)
		*exit_status = redirect_stdin(stdin_fd, input_fd);
	if (is_output && !*exit_status)
		*exit_status = redirect_stdout(stdout_fd, output_fd);
}

void	redirect(int *stdin_fd, int *stdout_fd, t_sep *node, int *exit_code)
{
	t_redirect	redirect;
	t_red		*red_head;

	redirect.is_input = 0;
	redirect.is_output = 0;
	while (node != NULL)
	{
		red_head = node->red;
		while (node->red != NULL)
		{
			if ((node->red->red_op == 'i' || node->red->red_op == 'h')
				&& (open_input(&redirect, node, exit_code)))
				break ;
			if ((node->red->red_op == 'o' || node->red->red_op == 'a')
				&& (open_output(&redirect, node->red->red_op, node, exit_code)))
				break ;
			node->red = node->red->next;
		}
		node->red = red_head;
		node = node->next;
	}
	redirect_fds((int *[4]){stdin_fd, &redirect.input_fd, stdout_fd,
		&redirect.output_fd}, redirect.is_input,
		redirect.is_output, exit_code);
}
