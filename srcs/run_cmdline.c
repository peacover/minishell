/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmdline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 15:34:01 by mhaddi            #+#    #+#             */
/*   Updated: 2021/07/09 15:30:31 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <errno.h>

int is_builtin(char *cmd)
{
	if (strcmp(cmd, "echo") == 0
	|| strcmp(cmd, "cd") == 0
	|| strcmp(cmd, "pwd") == 0
	|| strcmp(cmd, "export") == 0
	|| strcmp(cmd, "unset") == 0
	|| strcmp(cmd, "env") == 0
	|| strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

int echo(char **args)
{
	int no_newline;
	int i;

	if (!args)
	{
		printf("\n");
		return (0);
	}

	no_newline = 0;
	if (strcmp(args[0], "-n") == 0)
		no_newline = 1;

	i = no_newline;
	while (args[i]) // should end with null
	{
		printf("%s", args[i]);
		i++;
		if (args[i])
			printf(" ");
	}
	if (no_newline == 0)
		printf("\n");

	return (0);
}

void cd(char **args)
{
	if (!args || strcmp(args[0], "~") == 0)
	{
		if (chdir(getenv("HOME")) == -1)
			printf("%s\n", strerror(errno)); // to free
	}
	else
	{
		if (chdir(args[0]) == -1)
			printf("%s\n", strerror(errno));
	}
}

void pwd(void) {
    char* buffer;

	buffer = getcwd(NULL, 0);
	if (!buffer)
	{
		printf("Failed to get current directory.\n");
	}
	else
	{
		printf("%s\n", buffer);
		free(buffer);
	}
}

void    run_cmdline(t_sep *node)
{
	
	if (node->next == NULL) // no pipes or redirections
	{
		// printf("true, next is null.\n");
		if (is_builtin(node->builtin))
		{
			// printf("whoops, node is builtin?\n");
			if (strcmp(node->lower_builtin, "echo") == 0)
				echo(node->args);
			if (strcmp(node->lower_builtin, "cd") == 0)
				cd(node->args);
			if (strcmp(node->lower_builtin, "pwd") == 0)
				pwd();
			/*
			if (strcmp(node->lower_builtin, "export") == 0)
				export(node->args);
			if (strcmp(node->lower_builtin, "unset") == 0)
				unset(node->args);
			if (strcmp(node->lower_builtin, "env") == 0)
				env(node->args);
				*/
			if (strcmp(node->lower_builtin, "exit") == 0)
			{
				printf("exit\n");
				exit(0);
			}
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
