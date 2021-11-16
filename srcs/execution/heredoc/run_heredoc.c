/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_heredoc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 10:50:34 by mhaddi            #+#    #+#             */
/*   Updated: 2021/11/16 11:39:25 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	run_heredoc_child(int *input_fd, char *file_name, t_sep *node)
{
	g_data.is_forked = 1;
	*input_fd = open(file_name, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (check_error(*input_fd < 0, file_name, "minishell: open", -1))
		return (1);
	if (heredoc_loop(input_fd, node, file_name))
		return (1);
	if (check_error(close(*input_fd) < 0, file_name, "minishell: close", -1))
		return (1);
	exit(0);
}

int	heredoc_process(t_sep *node, int *exit_status, char *file_name)
{
	int		input_fd;
	pid_t	fork_pid;

	fork_pid = fork();
	if (check_error(fork_pid < 0, file_name, "minishell: fork", -1))
		return (1);
	if (fork_pid == 0)
		if (run_heredoc_child(&input_fd, file_name, node))
			return (1);
	if (check_error(waitpid(fork_pid, exit_status, 0) < 0,
			file_name, "minishell: waitpid", -1))
		return (1);
	*exit_status = WEXITSTATUS(*exit_status);
	g_data.is_forked = 0;
	if (check_error(signal(SIGINT, signal_handler_parent) == SIG_ERR,
			file_name, "minishell: signal", -1))
		return (1);
	free(node->red->r_file);
	node->red->r_file = file_name;
	return (0);
}

char	*create_heredoc_file(int *i)
{
	char	*count;
	char	*file_name;

	(*i)++;
	count = ft_itoa(*i);
	file_name = ft_strjoin("/tmp/.heredoc_", count);
	free(count);
	return (file_name);
}

int	run_heredoc_core(int *i, t_sep *node, int *exit_status)
{
	char	*file_name;

	if (!node->red->r_file || !*node->red->r_file)
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		return (258);
	}
	file_name = create_heredoc_file(i);
	if (check_error(signal(SIGINT, signal_handler_heredoc)
			== SIG_ERR, file_name, "minishell: signal", -1))
		return (1);
	if (heredoc_process(node, exit_status, file_name))
		return (1);
	return (0);
}

int	run_heredoc(t_sep *node)
{
	int		i;
	int		exit_code;
	t_red	*red_head;

	exit_code = 0;
	i = 0;
	while (node != NULL)
	{
		red_head = node->red;
		while (node->red != NULL)
		{
			if (node->red->red_op == 'h')
			{
				exit_code = run_heredoc_core(&i, node, &exit_code);
				if (exit_code)
					return (exit_code);
			}
			node->red = node->red->next;
		}
		node->red = red_head;
		node = node->next;
	}
	return (exit_code);
}
