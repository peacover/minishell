/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmdline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 15:34:01 by mhaddi            #+#    #+#             */
/*   Updated: 2021/07/10 20:37:37 by mhaddi           ###   ########.fr       */
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

void    ft_lstadd_front(t_env **alst, t_env *new)
{
    t_env  *temp;

    if (new)
    {
        temp = *alst;
        *alst = new;
        (*alst)->next = temp;
    }
    else
        *alst = new;
}

void    ft_lstadd_back(t_env **alst, t_env *new)
{
    t_env  *tmp;

    if (!*alst)
        ft_lstadd_front(alst, new);
    else
    {
        tmp = *alst;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new;
    }
}

void    ft_setenv(char *key, char *value)
{
   t_env *current = g_env;
   int key_exists = 0;

   while (current != NULL)
   {
	   if (ft_strcmp(current->key, key) == 0)
	   {
		   current->value = value;
		   key_exists = 1;
		   break ;
	   }
	   current = current->next;
   }

   int is_valid_identifier = 1;
   int i = 0;
   while (key[i])
   {
	   if (!ft_isalnum(key[i]) && key[i] != '_')
	   {
			is_valid_identifier = 0;
			break ;
	   }
	   i++;
   }

   if (!key_exists && is_valid_identifier)
   {
	   t_env *new_env_var;
	   new_env_var = malloc(sizeof(*new_env_var));
	   new_env_var->value
		   = ft_strjoin(
				   ft_strjoin(key, "="), value); // to refactor and free
	   new_env_var->key = key;
	   new_env_var->value= value;
	   new_env_var->next = NULL;
	   ft_lstadd_back(&g_env, new_env_var);
   }

   if (!is_valid_identifier)
	   printf("minishell: export: `%s': not a valid identifier\n", key);
}

void cd(char **args)
{
	ft_setenv("OLDPWD", getcwd(NULL, 0)); // TO-DO: only change its value
										  // when directory is actually
										  // changed (not cd to the same cwd)

	// TO-DO: cd -
	
	if (!args || strcmp(args[0], "~") == 0)
	{
		if (chdir(getenv("HOME")) == -1)
			printf("%s\n", strerror(errno));
		else
			ft_setenv("PWD", getcwd(NULL, 0));
	}
	else
	{
		if (chdir(args[0]) == -1)
			printf("%s\n", strerror(errno));
		else
			ft_setenv("PWD", getcwd(NULL, 0));
	}
}

void pwd(void) {
    char* cwd;

	cwd = getcwd(NULL, 0);
	printf("%s\n", cwd);
	free(cwd);
}

void    ft_putenv(char *env_var)
{
   t_env *current = g_env;
   char **env_var_pair = ft_split(env_var, '=');
   int key_exists = 0;
   while (current != NULL)
   {
	   if (ft_strcmp(current->key, env_var_pair[0]) == 0)
	   {
		   current->value = env_var_pair[1];
		   key_exists = 1;
		   break ;
	   }
	   current = current->next;
   }

   int is_valid_identifier = 1;
   int i = 0;
   while (env_var_pair[0][i])
   {
	   if (!ft_isalnum(env_var_pair[0][i]) && env_var_pair[0][i] != '_')
	   {
			is_valid_identifier = 0;
			break ;
	   }
	   i++;
   }

   if (!key_exists && is_valid_identifier)
   {
	   t_env *new_env_var;
	   new_env_var = malloc(sizeof(*new_env_var));
	   new_env_var->val = env_var;
	   new_env_var->key = env_var_pair[0];
	   new_env_var->value= env_var_pair[1];
	   new_env_var->next = NULL;
	   ft_lstadd_back(&g_env, new_env_var);
   }

   if (!is_valid_identifier)
	   printf("minishell: export: `%s': not a valid identifier\n",
				env_var_pair[0]);
}

// - it takes multiple arguments
// - assign value for each 
// - if it has no =, value is null, and key
// will be added but not printed when using env
// - if an invalid identifier is in the middle
// you continue with other valid identifiers
// and print an error at the end.
//
// - VALID IDENTIFIERS:
// name is a word consisting only of alphanumeric
// characters and under‐scores, and beginning
// with an alphabetic character or an
// under‐score.  Also referred to as an
// identifier.
void export(t_sep *node)
{
	int i = 0;

	if (node->args)
	{
		while (node->args[i])
		{
			ft_putenv(node->args[i]);
			i++;
		}
	}
	else
	{
		t_env *current = g_env;
		while (current != NULL)
		{
			printf("declare -x %s", current->key);
			if (current->value)
				printf("=\"%s\"", current->value);
			printf("\n");
			current = current->next;
		}
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
			if (strcmp(node->lower_builtin, "export") == 0)
				export(node);
			/*
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
				// execve(node->path, node->args, g_envp);
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
