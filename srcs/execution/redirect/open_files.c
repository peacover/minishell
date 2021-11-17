/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 11:29:06 by mhaddi            #+#    #+#             */
/*   Updated: 2021/11/17 15:44:49 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	check_redir_error(int condition, char *file)
{
	if (condition)
	{
		printf("minishell: %s: %s\n", file, strerror(errno));
		return (1);
	}
	return (0);
}

int	open_input(t_redirect *redirect, t_sep *node, int *exit_status)
{
	int	*input_fd;
	int	*is_input;

	input_fd = &redirect->input_fd;
	is_input = &redirect->is_input;
	*exit_status = 0;
	*input_fd = open(node->red->r_file, O_RDONLY);
	*exit_status = check_redir_error(*input_fd == -1, node->red->r_file);
	free(node->red->r_file);
	node->red->r_file = ft_itoa(*input_fd);
	(*is_input)++;
	return (*exit_status);
}

int	open_output(t_redirect *redirect, char type, t_sep *node, int *exit_status)
{
	int	*output_fd;
	int	*is_output;

	output_fd = &redirect->output_fd;
	is_output = &redirect->is_output;
	*exit_status = 0;
	if (type == 'o')
		*output_fd = open(node->red->r_file,
				O_CREAT | O_TRUNC | O_WRONLY, 0644);
	else if (type == 'a')
		*output_fd = open(node->red->r_file,
				O_CREAT | O_APPEND | O_WRONLY, 0644);
	if (check_redir_error(*output_fd == -1, node->red->r_file))
	{
		*exit_status = 1;
		return (*exit_status);
	}
	free(node->red->r_file);
	node->red->r_file = ft_itoa(*output_fd);
	(*is_output)++;
	return (*exit_status);
}
