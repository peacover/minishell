/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_redirect.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 11:25:32 by mhaddi            #+#    #+#             */
/*   Updated: 2021/11/16 11:25:58 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	dup_redirection(int i_red_found, int o_red_found,
		char *filenames[2], pid_t *pids)
{
	char	*i_red_file;
	char	*o_red_file;

	i_red_file = filenames[0];
	o_red_file = filenames[1];
	if (o_red_found)
		check_error(dup2(ft_atoi(o_red_file), 1) < 0,
			pids, "minishell: dup2", 1);
	if (i_red_found)
		check_error(dup2(ft_atoi(i_red_file), 0) < 0,
			pids, "minishell: dup2", 1);
	return (0);
}

char	*set_redir_file(int *red_found, char *red_file_src)
{
	*red_found = 1;
	return (red_file_src);
}

int	pipe_redirect(t_sep *node, pid_t *pids)
{
	int		i_red_found;
	char	*i_red_file;
	int		o_red_found;
	char	*o_red_file;

	i_red_found = 0;
	o_red_found = 0;
	while (node->red != NULL)
	{
		if (node->red->red_op == 'o' || node->red->red_op == 'a')
		{
			o_red_file = set_redir_file(&o_red_found, node->red->r_file);
			if (ft_strcmp(o_red_file, "-1") == 0)
				return (1);
		}
		else if (node->red->red_op == 'h' || node->red->red_op == 'i')
		{
			i_red_file = set_redir_file(&i_red_found, node->red->r_file);
			if (ft_strcmp(i_red_file, "-1") == 0)
				return (1);
		}
		node->red = node->red->next;
	}
	return (dup_redirection(i_red_found, o_red_found,
			(char *[2]){i_red_file, o_red_file}, pids));
}
