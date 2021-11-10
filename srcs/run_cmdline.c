/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmdline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 15:34:01 by mhaddi            #+#    #+#             */
/*   Updated: 2021/11/10 10:46:47 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <errno.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <dirent.h>

/*
char    *ft_getline(void)
{
    char    c;
    char    *line;
    char    *tmp;

    line = malloc(1);
    *line = '\0';
    while (1)
    {
        read(0, &c, 1);
        if (c == EOF)
            return (line);
        tmp = line;
        line = ft_strjoin(tmp, (char [2]){c, '\0'});
        free(tmp);
        if (c == '\n')
            return (line);
    }
}
*/

// printf("minishell: cd: %s: %s\n", args[0], strerror(errno));

char    *ft_getline(void)
{
    char    c;
    char    *line;
    char    *tmp;
	int		read_status;

    line = malloc(1);
    *line = '\0';
    while (1)
    {
        read_status = !read(0, &c, 1);
		if (!read_status)
        {
            tmp = line;
            line = ft_strjoin(tmp, "EOF");
            free(tmp);
            return (line);
        }
		else if (read_status < 0)
		{
			printf("minishell: read: %s\n", strerror(errno));
			free(line);
			line = NULL;
			return (line);
		}
        tmp = line;
        line = ft_strjoin(tmp, (char [2]){c, '\0'});
        free(tmp);
        if (c == '\n')
            return (line);
    }
}

int has_quotes(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\"' || str[i] == '\'')
			return (1);
		i++;
	}

	return (0);
}

int is_forked = 0;

int run_heredoc(t_sep *node)
{
	char *line;
	char *unexpanded_line;
	int input_fd;
	int i;
	char *file_name;
	char *count;
	int exit_status = 0;
	t_red *red_head;

	i = 0;
	while (node != NULL)
	{
		red_head = node->red;
		while (node->red != NULL)
		{
			if (node->red->red_op == 'h')
			{
				i++;
				count = ft_itoa(i);
				file_name = ft_strjoin("/tmp/.heredoc_", count); // to free
				free(count);
				if (signal(SIGINT, signal_handler_heredoc) == SIG_ERR)
				{
					printf("minishell: signal: %s\n", strerror(errno));
					free(file_name);
					return (1);
				}
				pid_t fork_pid = fork();
				if (fork_pid < 0)
				{
					printf("minishell: fork: %s\n", strerror(errno));
					free(file_name);
					return (1);
				}
				if (fork_pid == 0)
				{
					is_forked = 1;
					input_fd = open(file_name, O_WRONLY | O_TRUNC | O_CREAT, 0644);
					if (input_fd < 0)
					{
						printf("minishell: open: %s\n", strerror(errno));
						free(file_name);
						return (1);
					}
					while (1)
					{
						if (write(1, "> ", 2) < 0)
						{
							printf("minishell: write: %s\n", strerror(errno));
							free(file_name);
							return (1);
						}
						line = ft_getline(); 
						if (!line)
						{
							free(file_name);
							return (1);
						}
						if (ft_strcmp(line, "EOF") == 0)
						{
							free(line);
							break ;
						}
						char *delimiter = ft_strjoin(node->red->r_file, "\n");
						if (ft_strncmp(delimiter, line, ft_strlen(line)) == 0)
						{
							free(delimiter);
							free(line);
							break;
						}
						free(delimiter);
						// implement expansion if there is no "" in STOP word
						if (!has_quotes(line))  // this should be checked in parsing and added
							// as a new parameter to the redirection list node,
							// because quotes needs to be removed from the string.
						{
							unexpanded_line = line;
							line = handling_dollar(line);
							free(unexpanded_line);
						}
						if (write(input_fd, line, ft_strlen(line)) < 0)
						{
							printf("minishell: write: %s\n", strerror(errno));
							free(file_name);
							free(line);
							return (1);
						}
						free(line);
					}
					if (close(input_fd) < 0)
					{
						printf("minishell: close: %s\n", strerror(errno));
						free(file_name);
						return (1);
					}
					exit(0);
				}
				if (waitpid(fork_pid, &exit_status, 0) < 0)
				{
					printf("minishell: waitpid: %s\n", strerror(errno));
					free(file_name);
					return (1);
				}
				exit_status = WEXITSTATUS(exit_status);
				is_forked = 0;
				if (signal(SIGINT, signal_handler_parent) == SIG_ERR)
				{
					printf("minishell: signal: %s\n", strerror(errno));
					free(file_name);
					return (1);
				}
				// change redir type and file name here
				free(node->red->r_file);
				node->red->r_file = file_name;
			}
			node->red = node->red->next;
		}
		node->red = red_head;
		node = node->next;
	}
	return exit_status;
}

int echo(char **args)
{
	int no_newline;
	int i;

	if (!args || !*args || **args == '\0')
	{
		printf("\n");
		return (0);
	}

	no_newline = 0;
	if (ft_strcmp(args[0], "-n") == 0)
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

char    *ft_getenv(char *key)
{
   t_env *current = g_env;

   while (current != NULL)
   {
	   if (ft_strcmp(current->key, key) == 0)
		   return ft_strdup(current->value);
	   current = current->next;
   }
   return NULL;
}

int    ft_setenv(char *key, char *value)
{
	t_env *current = g_env;
	int key_exists = 0;
	char *key_eq;
	int exit_status = 0;

	while (current != NULL)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
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
		key_eq = ft_strjoin(key, "=");
		new_env_var->val = ft_strjoin(key_eq, value);
		free(key_eq);
		new_env_var->key = key;
		new_env_var->value = value;
		new_env_var->next = NULL;
		ft_lstadd_back(&g_env, new_env_var);
	}

	if (!is_valid_identifier)
	{
		printf("minishell: export: `%s': not a valid identifier\n", key);
		exit_status = 1;
	}

	return exit_status;
}

int    ft_unsetenv(char *key)
{
   int i = 0;
   t_env *current = g_env;
   t_env *tmp;
   t_env *prev;
   int exit_status = 0;

   if (*key == '\0')
   {
	   printf("minishell: unset: `%s': not a valid identifier\n", key);
	   exit_status = 1;
	   return (exit_status);
   }

   while (key[i])
   {
	   if (!ft_isalnum(key[i]) && key[i] != '_')
	   {
		   printf("minishell: unset: `%s': not a valid identifier\n", key);
	   	   exit_status = 1;
		   return (exit_status);
	   }
	   i++;
   }

   i = 0;
   while (current != NULL)
   {
	   if (ft_strcmp(current->key, key) == 0)
	   {
		   tmp = current->next;
		   if (i)
			   prev->next = tmp;
		   else
			   g_env = tmp;
		   free(current->val);
		   free(current->key);
		   free(current->value);
		   free(current);
		   return exit_status;
	   }
	   prev = current;
	   current = current->next;
	   i++;
   }

   return exit_status;
}

char *ft_tolower_str(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		str[i] = ft_tolower(str[i]);
		i++;
	}
	return str;
}

int	is_cwd(char *path)
{
	char *cwd = getcwd(NULL, 0);
	chdir(path);
	char *new_d = getcwd(NULL, 0);
	chdir(cwd);
	if (ft_strcmp(cwd, new_d))
		return 0;
	return 1;
}

int cd(char **args)
{
	char *cwd;
	int exit_status = 0;

	if (args && *args && **args == '~')
	{
		free(args[0]);
		args[0] = ft_getenv("HOME");
	}

	// set OLDPWD only if cmd's path:
	// - is not null
	// - is not `-`
	// - is a valid directory
	// - is not current directory // SIKE ?!!
	// OR:
	// - is null (so `cd`)
	// - current directory is not home directory

	if (args && *args)
	{
		if (**args == '\0')
		{
			cwd = getcwd(NULL, 0);
			exit_status = ft_setenv("OLDPWD", cwd);
			return exit_status;
		}

		if (ft_strcmp(args[0], "-") && opendir(args[0])) // && !is_cwd(args[0]))
		{
			cwd = getcwd(NULL, 0);
			if ((exit_status = ft_setenv("OLDPWD", cwd)) != 0)
				return exit_status;
		}

		if (ft_strcmp(args[0], "-") == 0)
		{
			char *oldpwd = ft_getenv("OLDPWD");
			if (!oldpwd)
			{
   				printf("minishell: cd: OLDPWD not set\n");
				exit_status = 1;
			}
			else
			{
				cd((char *[]){oldpwd, NULL});
				free(oldpwd);
				cwd = getcwd(NULL, 0);
				printf("%s\n", cwd);
				free(cwd);
			}
		}
		else if (chdir(args[0]) == -1)
		{
			printf("minishell: cd: %s: %s\n", args[0], strerror(errno));
			exit_status = 1;
		}
		else
		{
			cwd = getcwd(NULL, 0);
			if ((exit_status = ft_setenv("PWD", cwd)) != 0)
				return exit_status;
		}
	}
	else
	{
		// if (ft_strcmp(getenv("HOME"), getcwd(NULL, 0)))
		cwd = getcwd(NULL, 0);
		if ((exit_status = ft_setenv("OLDPWD", cwd)) != 0)
			return exit_status;
		if (chdir(getenv("HOME")) == -1)
		{
			printf("minishell: cd: %s\n", strerror(errno));
			exit_status = 1;
		}
		else
		{
			cwd = getcwd(NULL, 0);
			if ((exit_status = ft_setenv("PWD", cwd)) != 0)
				return exit_status;
		}
	}
	return exit_status;
}

int pwd(void) {
    char* cwd;
	int exit_status = 0;

	cwd = getcwd(NULL, 0);
	printf("%s\n", cwd);
	free(cwd);
	return exit_status;
}

char **split_identifier(char *env_var)
{
	int i = 0;

	if (env_var)
	{
		while (env_var[i])
		{
			if (env_var[i] == '=')	
				break ;
			i++;
		}
	}

	char *key = malloc(sizeof(*key) * (i + 1));
	ft_strlcpy(key, env_var, i + 1);
	char *value = NULL;
	if ((int)ft_strlen(env_var) > i)
	{
		value = malloc(sizeof(*value) * (ft_strlen(env_var) - i));
		ft_strlcpy(value, env_var + i + 1, ft_strlen(env_var) - i);
	}
	else if (env_var[i] == '=')
	{
		char *value = malloc(sizeof(*value));
		*value = '\0';
	}
	char **pair = malloc(sizeof(*pair) * 3);
	pair[0] = key;
	pair[1] = value;
	pair[2] = NULL;

	return pair;
}

int ft_putenv(char *env_var)
{
   t_env *current = g_env;
   int is_valid_identifier = 1;
   int key_exists = 0;
   int exit_status = 0;

   char **env_var_pair = split_identifier(env_var);
   if (env_var_pair[0][0] == '\0' || ft_isdigit(env_var_pair[0][0]))
	   is_valid_identifier = 0;
   else
   {
	   while (env_var_pair && current != NULL)
	   {
		   if (ft_strcmp(current->key, env_var_pair[0]) == 0)
		   {
			   if (env_var_pair[1]) // if key exists and the new value
				   					// isn't a null value
				   current->value = ft_strdup(env_var_pair[1]);
			   key_exists = 1;
			   break ;
		   }
		   current = current->next;
	   }

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
   }

   if (!key_exists && is_valid_identifier)
   {
	   t_env *new_env_var;
	   new_env_var = malloc(sizeof(*new_env_var));
	   new_env_var->val = env_var;
	   new_env_var->key = ft_strdup(env_var_pair[0]);
	   new_env_var->value= ft_strdup(env_var_pair[1]);
	   new_env_var->next = NULL;
	   ft_lstadd_back(&g_env, new_env_var);
   }

   if (!is_valid_identifier)
   {
	   printf("minishell: export: `%s': not a valid identifier\n",
				env_var);
	   exit_status = 1;
   }

   free(env_var_pair[0]);
   free(env_var_pair[1]);
   free(env_var_pair);

   return exit_status;
}

// export():
// - it takes multiple arguments
// - assign value for each 
// - if it has no =, value is null, and key
// will be added but not printed when using env
// - if an invalid identifier is in the middle
// you continue with other valid identifiers
// and print an error at the end.
//
// - WHAT ARE VALID IDENTIFIERS:
// name is a word consisting only of alphanumeric
// characters and under‐scores, and beginning
// with an alphabetic character or an
// under‐score.  Also referred to as an
// identifier.
//
int export(char **args)
{
	int i = 0;
	int exit_status = 0;

	if (args && *args)
	{
		while (args[i])
		{
			exit_status = ft_putenv(args[i]);
			i++;
		}
	}
	else
	{
		t_env *current = g_env;
		while (current != NULL)
		{
			if (current->key[0] != '?')
			{
				printf("declare -x %s", current->key);
				if (current->value)
					printf("=\"%s\"", current->value);
				printf("\n");
			}
			current = current->next;
		}
	}

	return exit_status;
}

int unset(char **args)
{
	int i = 0;
	int exit_status = 0;

	if (args)
	{
		while (args[i])
		{
			if (ft_unsetenv(args[i]) == 1)
				exit_status = 1;
			i++;
		}
	}
	return exit_status;
}

void env()
{
	t_env *current = g_env;
	while (current != NULL)
	{
		if (current->value && current->key[0] != '?')
			printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
}

int	redirect(int *stdin_fd, int *stdout_fd, t_sep *node)
{
	int input_fd;
	int output_fd;
	int is_input = 0;
	int is_output = 0;
	t_red *red_head;

	while (node != NULL)
	{
		red_head = node->red;
		while (node->red != NULL)
		{
			// check type of redir
			if (node->red->red_op == 'i' || node->red->red_op == 'h')
			{
				input_fd = open(node->red->r_file, O_RDONLY);
				if (input_fd == -1)
				{
					printf("minishell: %s: %s\n", node->red->r_file, strerror(errno));
					return (1);
				}
				free(node->red->r_file);
				node->red->r_file = ft_itoa(input_fd);
				is_input++;
			}
			else if (node->red->red_op == 'o')
			{
				output_fd = open(node->red->r_file, O_CREAT | O_TRUNC | O_WRONLY, 0644);
				if (output_fd == -1)
				{
					printf("minishell: %s: %s\n", node->red->r_file, strerror(errno));
					return (1);
				}
				free(node->red->r_file);
				node->red->r_file = ft_itoa(output_fd);
				is_output++;
			}
			else if (node->red->red_op == 'a')
			{
				output_fd = open(node->red->r_file, O_CREAT | O_APPEND | O_WRONLY, 0644);
				if (output_fd == -1)
				{
					printf("minishell: %s: %s\n", node->red->r_file, strerror(errno));
					return (1);
				}
				free(node->red->r_file);
				node->red->r_file = ft_itoa(output_fd);
				is_output++;
			}
			node->red = node->red->next;
		}
		node->red = red_head;
		node = node->next;
	}

	if (is_input)
	{
		*stdin_fd = dup(0);
		if (*stdin_fd < 0)
		{
			printf("minishell: dup: %s\n", strerror(errno));
			return (1);
		}
		if (dup2(input_fd, 0) < 0)
		{
			printf("minishell: dup2: %s\n", strerror(errno));
			return (1);
		}
	}
	if (is_output)
	{
		*stdout_fd = dup(1);
		if (*stdout_fd < 0)
		{
			printf("minishell: dup: %s\n", strerror(errno));
			return (1);
		}
		if (dup2(output_fd, 1) < 0)
		{
			printf("minishell: dup2: %s\n", strerror(errno));
			return (1);
		}
	}

	return (0);
}

// TO-DO: try out all possible usages of all the redirection
// operators (especially the various positions they can take),
// this might affect the parsing methods regarding the
// redirection operators.

void	signal_handler_heredoc(int sig)
{
	if (sig == SIGINT && is_forked)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		exit(1);
	}
}

void	signal_handler_parent(int sig)
{
	if (sig == SIGINT && is_forked)
	{
		write(1, "\n", 1);
		rl_on_new_line();
	}
	if (sig == SIGINT && !is_forked)
	{
		write(1, "\n", 1);
		rl_replace_line("", 1);
		rl_on_new_line();
		rl_redisplay();
		// update_status_code(1);
	}
}

int    run_cmdline(t_sep *node, int pipes_num)
{
	int stdin_fd = 0;
	int stdout_fd = 0;
	int exit_status = 0;

	if (run_heredoc(node) == 1)
		return (1);
	if (redirect(&stdin_fd, &stdout_fd, node))
		return (1);

	if (node->next == NULL) // no pipes or redirections
	{
		// printf("true, next is null.\n");
		if (node->is_builtin)
		{
			// printf("whoops, node is builtin?\n");
			if (ft_strcmp(node->lower_builtin, "echo") == 0)
				echo(node->args);
			if (ft_strcmp(node->lower_builtin, "cd") == 0)
				cd(node->args);
			if (ft_strcmp(node->lower_builtin, "pwd") == 0)
				pwd();
			if (ft_strcmp(node->lower_builtin, "export") == 0)
				exit_status = export(node->args);
			if (ft_strcmp(node->lower_builtin, "unset") == 0)
				exit_status = unset(node->args);
			if (ft_strcmp(node->lower_builtin, "env") == 0)
				env();
			if (ft_strcmp(node->lower_builtin, "exit") == 0)
			{
				printf("exit\n");
				exit(0); // return 0 and exit in main after free
			}
		}
		else if (node->path || node->builtin)
		{
			// printf("ok, we exec now:\n");
			// char *argv[] = {"/bin/sh", "-c", node->s_red, NULL};
			pid_t fork_pid = fork();
			if (fork_pid < 0)
			{
				printf("minishell: fork: %s\n", strerror(errno));
				return (1);
			}
			if (fork_pid == 0)
			{
				is_forked = 1;
				// execve(argv[0], argv, g_envp);
				if (execve((char *[2]){node->path, node->builtin}[!node->path], node->args, g_envp) == -1)
				{
					printf("minishell: %s: command not found\n", node->builtin);
					exit(1); // error code
				}
			}
			else
			{
				if (signal(SIGINT, SIG_IGN) == SIG_ERR)
				{
					printf("minishell: signal: %s\n", strerror(errno));
					return (1);
				}
				if (waitpid(fork_pid, &exit_status, 0) < 0) // TO-DO: handle exit codes in execve and builtins and others
				{
					printf("minishell: waitpid: %s\n", strerror(errno));
					return (1);
				}
				exit_status = WEXITSTATUS(exit_status);
				if (signal(SIGINT, signal_handler_parent) == SIG_ERR)
				{
					printf("minishell: signal: %s\n", strerror(errno));
					return (1);
				}
			}
		}

		if (dup2(stdin_fd, 0) < 0)
		{
			printf("minishell: dup2: %s\n", strerror(errno));
			return (1);
		}
		if (dup2(stdout_fd, 1) < 0)
		{
			printf("minishell: dup2: %s\n", strerror(errno));
			return (1);
		}
	}
	else // pipes and redirections
	{
		pid_t *pids = malloc(sizeof(pid_t) * (pipes_num + 1));
		int pipe_fd[2];
		if (pipe(pipe_fd) < 0)
		{
			free(pids);
			printf("minishell: pipe: %s\n", strerror(errno));
			return (1);
		}
		int num_cmd = 0;
		while (node != NULL)
		{
			// if sep is a pipe (e.g.: `ls | cat`, current node's cmd
			// is `ls` and next node's cmd is `cat`, their sep is `|`)
			// or if cmd is last cmd in the pipeline
			if (node->t_sp == '|' || num_cmd == pipes_num)
			{
				if (num_cmd > 0)
				{
					stdin_fd = dup(0);
					if (stdin_fd < 0 || dup2(pipe_fd[0], 0) < 0)
					{
						free(pids);
						printf("minishell: dup: %s\n", strerror(errno));
						return (1);
					}
					if (pipe(pipe_fd) < 0)
					{
						free(pids);
						printf("minishell: pipe: %s\n", strerror(errno));
						return (1);
					}
				}
				pids[num_cmd] = fork();
				if (pids[num_cmd] < 0)
				{
					free(pids);
					printf("minishell: fork: %s\n", strerror(errno));
					exit(1);
				}
				if (pids[num_cmd] == 0)
				{
					is_forked = 1;
					if (num_cmd < pipes_num && (node->path || node->builtin))
					{
						if (dup2(pipe_fd[1], 1) < 0)
						{
							free(pids);
							printf("minishell: dup2: %s\n", strerror(errno));
							exit(1);
						}
					}

					// check if there is a redir_op
					// if (node->is_red)
					// {
					// 	while (node->red->next != NULL)
					// 		node->red = node->red->next;
					// 	dup2(ft_atoi(node->red->r_file), 1);
					// 	// redirect(&stdin_fd, &stdout_fd, node);
					// }
					
					// check if there is a redir_op
					if (node->is_red)
					{
						int i_red_found = 0;
						char *i_red_file;
						int o_red_found = 0;
						char *o_red_file;
						while (node->red != NULL)
						{
							if (node->red->red_op == 'o' || node->red->red_op == 'a')
							{
								o_red_found = 1;
								o_red_file = node->red->r_file;
							}
							else if (node->red->red_op == 'h' || node->red->red_op == 'i')
							{
								i_red_found = 1;
								i_red_file = node->red->r_file;
							}
							node->red = node->red->next;
						}
						if (o_red_found)
						{
							if (dup2(ft_atoi(o_red_file), 1) < 0)
							{
								free(pids);
								printf("minishell: dup2: %s\n", strerror(errno));
								exit(1);
							}
						}
						if (i_red_found)
						{
							if (dup2(ft_atoi(i_red_file), 0) < 0)
							{
								free(pids);
								printf("minishell: dup2: %s\n", strerror(errno));
								exit(1);
							}
						}
						// redirect(&stdin_fd, &stdout_fd, node);
					}

					if (node->is_builtin)
					{
						if (ft_strcmp(node->lower_builtin, "echo") == 0)
							echo(node->args);
						if (ft_strcmp(node->lower_builtin, "cd") == 0)
							cd(node->args);
						if (ft_strcmp(node->lower_builtin, "pwd") == 0)
							pwd();
						if (ft_strcmp(node->lower_builtin, "export") == 0)
							export(node->args);
						if (ft_strcmp(node->lower_builtin, "unset") == 0)
							unset(node->args);
						if (ft_strcmp(node->lower_builtin, "env") == 0)
							env();
						if (ft_strcmp(node->lower_builtin, "exit") == 0)
						{
							exit(0); // return 0 and exit in main after free
						}
						/*
						char *argv[] = {"/bin/sh", "-c", node->s_red, NULL};
						if (execve(argv[0], argv, g_envp) == -1)
						{
							printf("minishell: %s: command not found\n", node->builtin);
							close(pipe_fd[1]);
							exit(1); // error code
						}
						*/
						exit(0);
					}
					else if (node->path || node->builtin)
					{
						if (execve((char *[2]){node->path, node->builtin}[!node->path], node->args, g_envp) == -1)
						{
							printf("minishell: %s: command not found\n", node->builtin);
							close(pipe_fd[1]);
							exit(1); // error code
						}
					}
					else
					{
						close(pipe_fd[1]);
						close(0);
						if (dup2(open("/dev/null", O_WRONLY), 1) < 0)
						{
							free(pids);
							printf("minishell: dup2: %s\n", strerror(errno));
							exit(1);
						}
					}
				}
				else {
					if (signal(SIGINT, SIG_IGN) == SIG_ERR)
					{
						free(pids);
						printf("minishell: signal: %s\n", strerror(errno));
						return (1);
					}
					if (waitpid(pids[num_cmd], &exit_status, 0) < 0)
					{
						free(pids);
						printf("minishell: waitpid: %s\n", strerror(errno));
						return (1);
					}
					exit_status = WEXITSTATUS(exit_status);
					if (signal(SIGINT, signal_handler_parent) == SIG_ERR)
					{
						free(pids);
						printf("minishell: signal: %s\n", strerror(errno));
						return (1);
					}
					close(pipe_fd[1]);
					num_cmd++;
				}
			}
			if (dup2(stdin_fd, 0) < 0 || dup2(stdout_fd, 1) < 0)
			{
				free(pids);
				printf("minishell: dup2: %s\n", strerror(errno));
				exit(1);
			}
			node = node->next;
		}
	}
	is_forked = 0;

	return exit_status;
}
