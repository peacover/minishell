/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_loop.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 11:30:42 by mhaddi            #+#    #+#             */
/*   Updated: 2021/11/16 11:39:09 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

char	*raise_error(char *line)
{
	printf("minishell: read: %s\n", strerror(errno));
	free(line);
	line = NULL;
	return (line);
}

char	*mark_eof(char *line)
{
	char	*tmp;

	tmp = line;
	line = ft_strjoin(tmp, "EOF");
	free(tmp);
	return (line);
}

char	*ft_getline(void)
{
	char	c;
	char	*line;
	char	*tmp;
	int		read_status;

	line = malloc(1);
	*line = '\0';
	while (1)
	{
		read_status = read(0, &c, 1);
		if (!read_status)
			return (mark_eof(line));
		else if (read_status < 0)
			return (raise_error(line));
		tmp = line;
		line = ft_strjoin(tmp, (char [2]){c, '\0'});
		free(tmp);
		if (c == '\n')
			return (line);
	}
}

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

	while (1)
	{
		if (check_error(write(1, "> ", 2) < 0,
				file_name, "minishell: write", -1))
			return (1);
		line = ft_getline();
		if (check_error(!line, file_name, NULL, -1))
			return (1);
		if (ft_strcmp(line, "EOF") == 0)
			break ;
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
