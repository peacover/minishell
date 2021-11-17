/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <yer-raki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 16:08:10 by yer-raki          #+#    #+#             */
/*   Updated: 2021/11/17 18:13:57 by yer-raki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ignctl(void)
{
	struct termios	term;

	if (tcgetattr(STDIN_FILENO, &term) != 0)
		perror("tcgetattr() error");
	term.c_lflag &= ~ECHOCTL;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &term) != 0)
		perror("tcsetattr() error");
}

void	main_init(int argc, char **argv, char **env)
{
	(void)argc;
	(void)argv;
	g_data.gg = NULL;
	g_data.envp = env;
	g_data.is_forked = 0;
	g_data.envl = fill_env(env);
}

void	signal_handlers(void)
{
	ignctl();
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, signal_handler_parent);
}

int	main(int argc, char **argv, char **env)
{
	char	*str;

	main_init(argc, argv, env);
	signal_handlers();
	while (1)
	{
		str = readline("$> ");
		if (!str)
		{
			write(2, "\b\bexit\n", 7);
			break ;
		}
		if (str[0] == '\0')
		{
			free(str);
			continue ;
		}
		add_history(str);
		if (!handling_errors_arg(str))
			fill_list(str);
		free(str);
	}
	free_garbage();
	return (0);
}
