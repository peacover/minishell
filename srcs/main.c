/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <yer-raki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 16:08:10 by yer-raki          #+#    #+#             */
/*   Updated: 2021/11/17 10:46:41 by yer-raki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

void	print_red(t_red *red)
{
	int i;

	i = 0;
	while (red)
	{
		printf(" \nRED_Node: %d\n", i);
		printf("\n---------------------------\n");
		printf (" File : %s", red->r_file);
		printf ("\n Type_red : %c", red->red_op);
		printf("\n---------------------------\n");
		red = red->next;
		i++;
	}
}

void    print_mylist(t_sep *node, int pipes_num)
{
	int i;
	int l;
	int l2;
	int node_num;

	i = 0;
	node_num = 0;
	while (node != NULL)
	{
		printf(" \nNode: %d\n", node_num);
		printf("\n------------------------------------------\n");
		printf (" path : %s", node->path);
		printf ("\n cmd : %s", node->builtin);
		printf ("\n is_builtin : %d", node->is_builtin);
		printf ("\n builtin : %s", node->builtin);
		printf ("\n s_red : %s", node->s_red);
		printf ("\n sep : %c", node->t_sp);
		printf ("\n pipes_num : %d", pipes_num);

		printf ("\n\n is_red : %d", node->is_red);
		print_red(node->red);
		i = 0; 
		l = ft_strlen2(node->args);
		l2 = ft_strlen2(node->r_args);
		while (l > i)
		{
			printf ("\n arg %d : |%s|", i, node->args[i]);
			i++;
		}
		// while (l2 > i)
		// {
		// 	printf ("\n r_arg %d : |%s|", i, node->r_args[i]);
		// 	i++;
		// }
		printf("\n------------------------------------------\n\n");
		node_num++;
		node = node->next;
	}
}

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
