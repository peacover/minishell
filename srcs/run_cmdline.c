/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmdline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <yer-raki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/08 15:34:01 by mhaddi            #+#    #+#             */
/*   Updated: 2021/11/13 23:23:58 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/types.h>

int	check_error(int condition, void *to_free, char *to_print, int exit_code)
{
	if (condition)
	{
		if (to_free)
			free(to_free);
		if (to_print)
			printf("%s: %s\n", to_print, strerror(errno));
		if (exit_code > -1)
			exit(exit_code);
		return (1);
	}
	return (0);
}

char	*mark_eof(char *line)
{
	char	*tmp;

	tmp = line;
	line = ft_strjoin(tmp, "EOF");
	free(tmp);
	return (line);
}

char	*raise_error(char *line)
{
	printf("minishell: read: %s\n", strerror(errno));
	free(line);
	line = NULL;
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

int	has_quotes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\"' || str[i] == '\'')
			return (1);
		i++;
	}
	return (0);
}

int	check_heredoc_syntax(int condition)
{
	if (condition)
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		return (258);
	}
	return (0);
}

char	*create_heredoc_file(int *i)
{
	char	*count;
	char	*file_name;

	(*i)++;
	count = ft_itoa(*i);
	file_name = ft_strjoin("/tmp/.heredoc_", count);
	free(count);
	return (file_name);
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

int	run_heredoc_child(int *input_fd, char *file_name, t_sep *node)
{
	g_data.is_forked = 1;
	*input_fd = open(file_name, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (check_error(*input_fd < 0, file_name, "minishell: open", -1))
		return (1);
	if (heredoc_loop(input_fd, node, file_name))
		return (1);
	if (check_error(close(*input_fd) < 0, file_name, "minishell: close", -1))
		return (1);
	exit(0);
}

int	heredoc_process(t_sep *node, int *exit_status, char *file_name)
{
	int		input_fd;
	pid_t	fork_pid;

	fork_pid = fork();
	if (check_error(fork_pid < 0, file_name, "minishell: fork", -1))
		return (1);
	if (fork_pid == 0)
		if (run_heredoc_child(&input_fd, file_name, node))
			return (1);
	if (check_error(waitpid(fork_pid, exit_status, 0) < 0,
			file_name, "minishell: waitpid", -1))
		return (1);
	*exit_status = WEXITSTATUS(*exit_status);
	g_data.is_forked = 0;
	if (check_error(signal(SIGINT, signal_handler_parent) == SIG_ERR,
			file_name, "minishell: signal", -1))
		return (1);
	free(node->red->r_file);
	node->red->r_file = file_name;
	return (0);
}

int	run_heredoc_core(int *i, t_sep *node, int *exit_status)
{
	char	*file_name;

	if (check_heredoc_syntax(!node->red->r_file
			|| !*node->red->r_file))
		return (258);
	file_name = create_heredoc_file(i);
	if (check_error(signal(SIGINT, signal_handler_heredoc)
			== SIG_ERR, file_name, "minishell: signal", -1))
		return (1);
	if (heredoc_process(node, exit_status, file_name))
		return (1);
	return (0);
}

int	run_heredoc(t_sep *node)
{
	int		i;
	int		exit_code;
	t_red	*red_head;

	exit_code = 0;
	i = 0;
	while (node != NULL)
	{
		red_head = node->red;
		while (node->red != NULL)
		{
			if (node->red->red_op == 'h')
			{
				exit_code = run_heredoc_core(&i, node, &exit_code);
				if (exit_code)
					return (exit_code);
			}
			node->red = node->red->next;
		}
		node->red = red_head;
		node = node->next;
	}
	return (exit_code);
}

int	echo(char **args)
{
	int	no_newline;
	int	i;

	if (!args)
	{
		printf("\n");
		return (0);
	}
	no_newline = 0;
	if (ft_strcmp(args[0], "-n") == 0)
		no_newline = 1;
	i = no_newline;
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i][0])
			if (args[i + 1] && args[i + 1][0])
				printf(" ");
		i++;
	}
	if (no_newline == 0)
		printf("\n");
	return (0);
}

void	ft_lstadd_front(t_env **alst, t_env *new)
{
	t_env	*temp;

	if (new)
	{
		temp = *alst;
		*alst = new;
		(*alst)->next = temp;
	}
	else
		*alst = new;
}

void	ft_lstadd_back(t_env **alst, t_env *new)
{
	t_env	*tmp;

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

char	*ft_getenv(char *key)
{
	t_env	*current;

	current = g_data.envl;
	while (current != NULL)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (ft_strdup(current->value));
		current = current->next;
	}
	return (NULL);
}

void	set_value_for_key(char *key, char *value, int *key_exists)
{
	t_env	*current;

	current = g_data.envl;
	while (current != NULL)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = value;
			*key_exists = 1;
			break ;
		}
		current = current->next;
	}
}

int	is_valid_identifier(char *key)
{
	int	i;

	i = 0;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	set_new_key(char *key, char *value)
{
	t_env	*new_env_var;
	char	*key_eq;

	new_env_var = malloc(sizeof(*new_env_var));
	key_eq = ft_strjoin(key, "=");
	new_env_var->val = ft_strjoin(key_eq, value);
	free(key_eq);
	new_env_var->key = key;
	new_env_var->value = value;
	new_env_var->next = NULL;
	ft_lstadd_back(&g_data.envl, new_env_var);
}

int	ft_setenv(char *key, char *value)
{
	int	key_exists;
	int	exit_status;
	int	is_valid_key;

	key_exists = 0;
	exit_status = 0;
	set_value_for_key(key, value, &key_exists);
	is_valid_key = is_valid_identifier(key);
	if (!key_exists && is_valid_key)
		set_new_key(key, value);
	else if (!is_valid_key)
	{
		printf("minishell: export: `%s': not a valid identifier\n", key);
		exit_status = 1;
	}
	return (exit_status);
}

int	check_key_syntax(int condition, char *key)
{
	if (condition)
	{
		printf("minishell: unset: `%s': not a valid identifier\n", key);
		return (1);
	}
	return (0);
}

int	unset_key(int i, t_env *current, t_env *prev)
{
	t_env	*tmp;

	tmp = current->next;
	if (i)
		prev->next = tmp;
	else
		g_data.envl = tmp;
	free(current->val);
	free(current->key);
	free(current->value);
	free(current);
	return (0);
}

int	ft_unsetenv(char *key)
{
	int		i;
	t_env	*current;
	t_env	*prev;

	i = 0;
	current = g_data.envl;
	if (check_key_syntax(*key == '\0', key))
		return (1);
	while (key[i])
	{
		if (check_key_syntax(!ft_isalnum(key[i]) && key[i] != '_', key))
			return (1);
		i++;
	}
	i = 0;
	while (current != NULL)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (unset_key(i, current, prev));
		prev = current;
		current = current->next;
		i++;
	}
	return (0);
}

char	*ft_tolower_str(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		str[i] = ft_tolower(str[i]);
		i++;
	}
	return (str);
}

int	is_cwd(char *path)
{
	char	*cwd;
	char	*new_d;

	cwd = getcwd(NULL, 0);
	chdir(path);
	new_d = getcwd(NULL, 0);
	chdir(cwd);
	if (ft_strcmp(cwd, new_d))
		return (0);
	return (1);
}

void	check_tilde(char **args)
{
	if (args && *args && **args == '~')
	{
		free(args[0]);
		args[0] = ft_getenv("HOME");
	}
}

int	check_valid_dir(char **args, char *cwd)
{
	int	exit_status;

	exit_status = 0;
	if (ft_strcmp(args[0], "-") && opendir(args[0]))
	{
		cwd = getcwd(NULL, 0);
		exit_status = ft_setenv("OLDPWD", cwd);
	}
	return (exit_status);
}

int		cd(char **args);

void	goto_oldpwd(int *exit_status, char *cwd)
{
	char	*oldpwd;

	oldpwd = ft_getenv("OLDPWD");
	if (!oldpwd)
	{
		printf("minishell: cd: OLDPWD not set\n");
		*exit_status = 1;
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

int	do_cd_with_args(char **args, char *cwd)
{
	int	exit_status;

	exit_status = 0;
	if (**args == '\0')
		return (ft_setenv("OLDPWD", getcwd(NULL, 0)));
	exit_status = check_valid_dir(args, cwd);
	if (exit_status)
		return (exit_status);
	if (ft_strcmp(args[0], "-") == 0)
		goto_oldpwd(&exit_status, cwd);
	else if (chdir(args[0]) == -1)
	{
		printf("minishell: cd: %s: %s\n", args[0], strerror(errno));
		exit_status = 1;
	}
	else
	{
		cwd = getcwd(NULL, 0);
		exit_status = ft_setenv("PWD", cwd);
	}
	return (exit_status);
}

int	do_cd_with_noargs(char *cwd)
{
	int	exit_status;

	exit_status = 0;
	cwd = getcwd(NULL, 0);
	exit_status = ft_setenv("OLDPWD", cwd);
	if (exit_status)
		return (exit_status);
	if (chdir(getenv("HOME")) == -1)
	{
		printf("minishell: cd: %s\n", strerror(errno));
		exit_status = 1;
	}
	else
	{
		cwd = getcwd(NULL, 0);
		exit_status = ft_setenv("PWD", cwd);
	}
	return (exit_status);
}

int	cd(char **args)
{
	char	*cwd;
	int		exit_status;

	cwd = NULL;
	exit_status = 0;
	check_tilde(args);
	if (args && *args)
		exit_status = do_cd_with_args(args, cwd);
	else
		exit_status = do_cd_with_noargs(cwd);
	return (exit_status);
}

void	pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	printf("%s\n", cwd);
	free(cwd);
}

int	get_index(char *env_var)
{
	int	i;

	i = 0;
	if (env_var)
	{
		while (env_var[i])
		{
			if (env_var[i] == '=')
				break ;
			i++;
		}
	}
	return (i);
}

char	**split_identifier(char *env_var)
{
	int		i;
	char	*key;
	char	*value;
	char	**pair;

	i = get_index(env_var);
	key = malloc(sizeof(*key) * (i + 1));
	ft_strlcpy(key, env_var, i + 1);
	value = NULL;
	if ((int)ft_strlen(env_var) > i)
	{
		value = malloc(sizeof(*value) * (ft_strlen(env_var) - i));
		ft_strlcpy(value, env_var + i + 1, ft_strlen(env_var) - i);
	}
	else if (env_var[i] == '=')
	{
		value = malloc(sizeof(*value));
		*value = '\0';
	}
	pair = malloc(sizeof(*pair) * 3);
	pair[0] = key;
	pair[1] = value;
	pair[2] = NULL;
	return (pair);
}

void	put_new_key(char *env_var, char **env_var_pair)
{
	t_env	*new_env_var;

	new_env_var = malloc(sizeof(*new_env_var));
	new_env_var->val = env_var;
	new_env_var->key = ft_strdup(env_var_pair[0]);
	new_env_var->value = ft_strdup(env_var_pair[1]);
	new_env_var->next = NULL;
	ft_lstadd_back(&g_data.envl, new_env_var);
}

void	lookup_key(char **env_var_pair,
				t_env *current,
				int *key_exists,
				int *is_valid_key)
{
	while (env_var_pair && current != NULL)
	{
		if (ft_strcmp(current->key, env_var_pair[0]) == 0)
		{
			if (env_var_pair[1])
				current->value = ft_strdup(env_var_pair[1]);
			*key_exists = 1;
			break ;
		}
		current = current->next;
	}
	*is_valid_key = is_valid_identifier(env_var_pair[0]);
}

void	free_pair(char **pair)
{
	int	i;

	i = 0;
	while (i < 2)
	{
		free(pair[i]);
		i++;
	}
	free(pair);
}

int	ft_putenv(char *env_var)
{
	t_env	*current;
	int		is_valid_key;
	int		key_exists;
	int		exit_status;
	char	**env_var_pair;

	current = g_data.envl;
	is_valid_key = 1;
	key_exists = 0;
	exit_status = 0;
	env_var_pair = split_identifier(env_var);
	if (env_var_pair[0][0] == '\0' || ft_isdigit(env_var_pair[0][0]))
		is_valid_key = 0;
	else
		lookup_key(env_var_pair, current, &key_exists, &is_valid_key);
	if (!key_exists && is_valid_key)
		put_new_key(env_var, env_var_pair);
	if (!is_valid_key)
	{
		printf("minishell: export: `%s': not a valid identifier\n", env_var);
		exit_status = 1;
	}
	free_pair(env_var_pair);
	return (exit_status);
}

int	do_export_with_args(char **args)
{
	int		i;
	int		exit_status;

	i = 0;
	exit_status = 0;
	while (args[i])
	{
		exit_status = ft_putenv(args[i]);
		i++;
	}
	return (exit_status);
}

void	do_export_with_noargs(void)
{
	t_env	*current;

	current = g_data.envl;
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

int	export(char **args)
{
	int		exit_status;

	exit_status = 0;
	if (args && *args)
		exit_status = do_export_with_args(args);
	else
		do_export_with_noargs();
	return (exit_status);
}

int	unset(char **args)
{
	int		i;
	int		exit_status;

	i = 0;
	exit_status = 0;
	if (args)
	{
		while (args[i])
		{
			if (ft_unsetenv(args[i]) == 1)
				exit_status = 1;
			i++;
		}
	}
	return (exit_status);
}

void	env(void)
{
	t_env	*current;

	current = g_data.envl;
	while (current != NULL)
	{
		if (current->value && current->key[0] != '?')
			printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
}

int	check_redir_error(int condition, char *file)
{
	if (condition)
	{
		printf("minishell: %s: %s\n", file, strerror(errno));
		return (1);
	}
	return (0);
}

int	open_input(t_redirect *redirect, t_sep *node, int *exit_status)
{
	int	*input_fd;
	int	*is_input;

	input_fd = &redirect->input_fd;
	is_input = &redirect->is_input;
	*exit_status = 0;
	*input_fd = open(node->red->r_file, O_RDONLY);
	*exit_status = check_redir_error(*input_fd == -1, node->red->r_file);
	free(node->red->r_file);
	node->red->r_file = ft_itoa(*input_fd);
	(*is_input)++;
	return (*exit_status);
}

int	open_output(t_redirect *redirect, char type, t_sep *node, int *exit_status)
{
	int	*output_fd;
	int	*is_output;

	output_fd = &redirect->output_fd;
	is_output = &redirect->is_output;
	*exit_status = 0;
	if (type == 'o')
		*output_fd = open(node->red->r_file,
				O_CREAT | O_TRUNC | O_WRONLY, 0644);
	else if (type == 'a')
		*output_fd = open(node->red->r_file,
				O_CREAT | O_APPEND | O_WRONLY, 0644);
	if (check_redir_error(*output_fd == -1, node->red->r_file))
	{
		*exit_status = 1;
		return (*exit_status);
	}
	free(node->red->r_file);
	node->red->r_file = ft_itoa(*output_fd);
	(*is_output)++;
	return (*exit_status);
}

int	redirect_stdin(int *stdin_fd, int *input_fd)
{
	*stdin_fd = dup(0);
	if (*stdin_fd < 0)
	{
		printf("minishell: dup: %s\n", strerror(errno));
		return (1);
	}
	if (dup2(*input_fd, 0) < 0)
	{
		printf("minishell: dup2: %s\n", strerror(errno));
		return (1);
	}
	return (0);
}

int	redirect_stdout(int *stdout_fd, int *output_fd)
{
	*stdout_fd = dup(1);
	if (*stdout_fd < 0)
	{
		printf("minishell: dup: %s\n", strerror(errno));
		return (1);
	}
	if (dup2(*output_fd, 1) < 0)
	{
		printf("minishell: dup2: %s\n", strerror(errno));
		return (1);
	}
	return (0);
}

void	redirect_fds(int *fds[4], int is_input, int is_output, int *exit_status)
{
	int	*stdin_fd;
	int	*input_fd;
	int	*stdout_fd;
	int	*output_fd;

	stdin_fd = fds[0];
	input_fd = fds[1];
	stdout_fd = fds[2];
	output_fd = fds[3];
	if (is_input && !*exit_status)
		*exit_status = redirect_stdin(stdin_fd, input_fd);
	if (is_output && !*exit_status)
		*exit_status = redirect_stdout(stdout_fd, output_fd);
}

void	redirect(int *stdin_fd, int *stdout_fd, t_sep *node, int *exit_code)
{
	t_redirect	redirect;
	t_red		*red_head;

	redirect.is_input = 0;
	redirect.is_output = 0;
	while (node != NULL)
	{
		red_head = node->red;
		while (node->red != NULL)
		{
			if ((node->red->red_op == 'i' || node->red->red_op == 'h')
				&& (open_input(&redirect, node, exit_code)))
				break ;
			if ((node->red->red_op == 'o' || node->red->red_op == 'a')
				&& (open_output(&redirect, node->red->red_op, node, exit_code)))
				break ;
			node->red = node->red->next;
		}
		node->red = red_head;
		node = node->next;
	}
	redirect_fds((int *[4]){stdin_fd, &redirect.input_fd, stdout_fd,
		&redirect.output_fd}, redirect.is_input,
		redirect.is_output, exit_code);
}

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

int	run_builtins(t_sep *node, int is_in_pipe)
{
	int		exit_status;

	exit_status = 0;
	if (ft_strcmp(node->lower_builtin, "echo") == 0)
		exit_status = echo(node->args);
	if (ft_strcmp(node->lower_builtin, "cd") == 0)
		exit_status = cd(node->args);
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
		if (!is_in_pipe)
			printf("exit\n");
		exit(0);
	}
	if (is_in_pipe)
		exit(0);
	return (exit_status);
}

void	run_child(t_sep *node)
{
	g_data.is_forked = 1;
	if (execve((char *[2]){node->path, node->builtin}[!node->path],
				node->args, g_data.envp) == -1)
	{
		printf("minishell: %s: command not found\n", node->builtin);
		exit(127);
	}
}

int	run_executable(t_sep *node)
{
	int		exit_status;
	pid_t	fork_pid;

	exit_status = 0;
	fork_pid = fork();
	if (check_error(fork_pid < 0, NULL, "minishell: fork", -1))
		return (1);
	if (fork_pid == 0)
		run_child(node);
	else
	{
		if (check_error(signal(SIGINT, SIG_IGN) == SIG_ERR,
				NULL, "minishell: signal", -1))
			return (1);
		if (check_error(waitpid(fork_pid, &exit_status, 0) < 0,
				NULL, "minishell: waitpid", -1))
			return (1);
		exit_status = WEXITSTATUS(exit_status);
		if (check_error(signal(SIGINT, signal_handler_parent) == SIG_ERR,
				NULL, "minishell: signal", -1))
			return (1);
	}
	return (exit_status);
}

int	run_no_pipe_cmd(t_sep *node, int *stdin_fd, int *stdout_fd)
{
	int	exit_status;

	exit_status = 0;
	if (node->is_builtin)
		exit_status = run_builtins(node, 0);
	else if (node->path || node->builtin)
		exit_status = run_executable(node);
	if (dup2(*stdin_fd, 0) < 0)
	{
		printf("minishell: dup2: %s\n", strerror(errno));
		return (1);
	}
	if (dup2(*stdout_fd, 1) < 0)
	{
		printf("minishell: dup2: %s\n", strerror(errno));
		return (1);
	}
	return (exit_status);
}

int	create_pipe(int *stdin_fd, int pipe_fd[2], pid_t *pids)
{
	*stdin_fd = dup(0);
	if (check_error(*stdin_fd < 0 || dup2(pipe_fd[0], 0) < 0,
			pids, "minishell: dup", -1))
		return (1);
	if (check_error(pipe(pipe_fd) < 0, pids, "minishell: pipe", -1))
		return (1);
	return (0);
}

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

void	run_pipe_executable(t_sep *node, int pipe_fd[2])
{
	if (execve((char *[2]){node->path, node->builtin}[!node->path],
				node->args,
				g_data.envp) == -1)
	{
		printf("minishell: %s: command not found\n", node->builtin);
		close(pipe_fd[1]);
		exit(127);
	}
}

void	case_no_cmd(int pipe_fd[2], pid_t *pids)
{
	close(pipe_fd[1]);
	close(0);
	check_error(
		dup2(open("/dev/null", O_WRONLY), 1) < 0, pids, "minishell: dup2", 1);
}

void	run_piped_process(int *ptrs[3], t_sep *node,
		int pipe_fd[2], pid_t *pids)
{
	int	*num_cmd;
	int	*pipes_num;
	int	*exit_status;

	num_cmd = ptrs[0];
	pipes_num = ptrs[1];
	exit_status = ptrs[2];
	g_data.is_forked = 1;
	if (*num_cmd < *pipes_num && (node->path || node->builtin))
		check_error(dup2(pipe_fd[1], 1) < 0, pids, "minishell: dup2", 1);
	if (node->is_red)
		if (pipe_redirect(node, pids) == 1)
			exit(1);
	if (node->is_builtin)
		*exit_status = run_builtins(node, 1);
	else if (node->path || node->builtin)
		run_pipe_executable(node, pipe_fd);
}

int	run_parent_process(pid_t *pids, int *num_cmd,
		int *exit_status, int pipe_fd[2])
{
	if (check_error(
			signal(SIGINT, SIG_IGN) == SIG_ERR, pids, "minishell: signal", -1))
		return (1);
	if (check_error(waitpid(pids[*num_cmd], exit_status, 0) < 0,
			pids, "minishell: waitpid", -1))
		return (1);
	*exit_status = WEXITSTATUS(*exit_status);
	if (check_error(signal(SIGINT, signal_handler_parent) == SIG_ERR,
			pids, "minishell: signal", -1))
		return (1);
	close(pipe_fd[1]);
	(*num_cmd)++;
	return (0);
}

int	run_piped_cmd(int *ptrs[4], int pipe_fd[2], pid_t *pids, t_sep *node)
{
	int	*num_cmd;
	int	*pipes_num;
	int	*exit_status;
	int	*stdin_fd;

	num_cmd = ptrs[0];
	pipes_num = ptrs[1];
	exit_status = ptrs[2];
	stdin_fd = ptrs[3];
	if (*num_cmd > 0)
		create_pipe(stdin_fd, pipe_fd, pids);
	pids[*num_cmd] = fork();
	check_error(pids[*num_cmd] < 0, pids, "minishell: fork", 1);
	if (pids[*num_cmd] == 0)
		run_piped_process(
			(int *[3]){num_cmd, pipes_num, exit_status}, node, pipe_fd, pids);
	else
	{
		if (run_parent_process(pids, num_cmd, exit_status, pipe_fd))
			return (1);
	}
	return (0);
}

int	run_pipes(t_sep *node, int pipes_num, int *stdin_fd, int *stdout_fd)
{
	int		exit_status;
	pid_t	*pids;
	int		pipe_fd[2];
	int		num_cmd;

	exit_status = 0;
	pids = malloc(sizeof(pid_t) * (pipes_num + 1));
	if (check_error(pipe(pipe_fd) < 0, pids, "minishell: pipe", -1))
		return (1);
	num_cmd = 0;
	while (node != NULL)
	{
		if (node->t_sp == '|' || num_cmd == pipes_num)
			if (run_piped_cmd((int *[4]){&num_cmd, &pipes_num,
					&exit_status, stdin_fd}, pipe_fd, pids, node))
				return (1);
		check_error(dup2(*stdin_fd, 0) < 0 || dup2(*stdout_fd, 1) < 0,
			pids, "minishell: dup2", 1);
		node = node->next;
	}
	return (exit_status);
}

int	run_cmdline(t_sep *node, int pipes_num)
{
	int		stdin_fd;
	int		stdout_fd;
	int		exit_status;

	stdin_fd = 0;
	stdout_fd = 0;
	exit_status = run_heredoc(node);
	if (exit_status)
		return (exit_status);
	redirect(&stdin_fd, &stdout_fd, node, &exit_status);
	if (node->next == NULL && !exit_status)
		exit_status = run_no_pipe_cmd(node, &stdin_fd, &stdout_fd);
	else if (node->next)
		exit_status = run_pipes(node, pipes_num, &stdin_fd, &stdout_fd);
	g_data.is_forked = 0;
	return (exit_status);
}
