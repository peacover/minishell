/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_loop.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <yer-raki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 11:30:42 by mhaddi            #+#    #+#             */
/*   Updated: 2021/11/17 11:24:44 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	is_delimiter(char *line, t_sep *node)
{
	char	*delimiter;

	delimiter = ft_strjoin(node->red->r_file, "\n");
	if (ft_strncmp(delimiter, line, ft_strlen(line)) == 0)
	{
		free(delimiter);
		return (1);
	}
	free(delimiter);
	return (0);
}

int	heredoc_loop(int *input_fd, t_sep *node, char *file_name)
{
	char	*line;
	char	*tmp;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		tmp = line;
		line = ft_strjoin(line, "\n");
		free(tmp);
		if (is_delimiter(line, node))
			break ;
		if (write(*input_fd, line, ft_strlen(line)) < 0)
		{
			printf("minishell: write: %s\n", strerror(errno));
			free(file_name);
			free(line);
			return (1);
		}
		free(line);
	}
	free(line);
	return (0);
}
