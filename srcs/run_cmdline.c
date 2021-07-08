/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmdline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 15:34:01 by mhaddi            #+#    #+#             */
/*   Updated: 2021/07/08 16:07:48 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void    run_cmdline(t_sep *node)
{
	
	if (node->next == NULL) // no pipes or redirections
	{
		// printf("true, next is null.\n");
		if (node->is_builtin)
		{
			// printf("whoops, node is builtin?\n");
		}
		else
		{
			// printf("ok, we exec now:\n");
			char *argv[] = {"/bin/sh", "-c", node->s_red, NULL};
			int fs = fork();
			if (fs == 0)
				execve(argv[0], argv, g_envp);
			waitpid(fs, NULL, 0);
		}
	}

	/*
	int i;
	int l;
	int node_num;
	
	i = 0;
	l = ft_strlen2(node->args);
	node_num = 0;
	while (node != NULL)
	{
		printf("\n\nNode: %d\n", node_num);
		printf("\n------------------------------------------\n");
		printf ("\n path : %s", node->path);
		printf ("\n cmd : %s", node->builtin);
		printf ("\n s_red : %s", node->s_red);
		i = 0; 
		while (l > i)
		{
			printf ("\n arg %d : %s", i, node->args[i]);
			i++;
		}
		printf("\n------------------------------------------\n");
		node_num++;
		node = node->next;
	}
	*/
}
