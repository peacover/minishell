/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 16:33:54 by mhaddi            #+#    #+#             */
/*   Updated: 2021/11/16 16:34:14 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
