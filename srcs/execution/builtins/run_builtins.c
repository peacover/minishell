/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_builtins.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 11:12:42 by mhaddi            #+#    #+#             */
/*   Updated: 2021/11/16 11:37:24 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

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
		if (args[i + 1])
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

void	set_exit_code(int value)
{
	t_env	*current;

	current = g_data.envl;
	while (current != NULL)
	{
		if (ft_strcmp(current->key, "?") == 0)
		{
			free(current->value);
			current->value = ft_itoa(value);
			break ;
		}
		current = current->next;
	}
}

char	*get_last_exit_code(void)
{
	t_env	*current;

	current = g_data.envl;
	while (current != NULL)
	{
		if (ft_strcmp(current->key, "?") == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

int	is_number(char *s)
{
	int	i;

	i = 0;
	if (s[i] == '-' || s[i] == '+')
		i++;
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (0);
		i++;
	}
	return (1);
}

void	exit_when_arg_is_non_numeric(int is_in_pipe, char *arg)
{
	set_exit_code(255);
	if (!is_in_pipe)
		printf("exit\n");
	printf("minishell: exit: %s: numeric argument required\n", arg);
	exit(255);
}

int	exit_when_multiple_args(int is_in_pipe, char *arg)
{
	if (is_number(arg))
	{
		set_exit_code(1);
		if (!is_in_pipe)
			printf("exit\n");
		printf("minishell: exit: too many arguments\n");
		return (1);
	}
	else
		exit_when_arg_is_non_numeric(is_in_pipe, arg);
	return (0);
}

void	exit_when_single_arg(int is_in_pipe, char *arg)
{
	int	number;

	if (is_number(arg))
	{
		number = ft_atoi(arg);
		while (number > 256)
			number -= 256;
		while (number < 0)
			number += 256;
		set_exit_code(number);
		if (!is_in_pipe)
			printf("exit\n");
		exit(number);
	}
	else
		exit_when_arg_is_non_numeric(is_in_pipe, arg);
}

int	ft_exit(char **args, int is_in_pipe)
{
	int	args_num;

	args_num = 0;
	if (args)
		while (args[args_num])
			args_num++;
	if (args_num > 1)
		if (exit_when_multiple_args(is_in_pipe, args[0]))
			return (1);
	if (args_num == 1)
		exit_when_single_arg(is_in_pipe, args[0]);
	else
	{
		if (!is_in_pipe)
			printf("exit\n");
		exit(ft_atoi(get_last_exit_code()));
	}
	return (0);
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
		exit_status = ft_exit(node->args, is_in_pipe);
	if (is_in_pipe)
		exit(ft_atoi(get_last_exit_code()));
	return (exit_status);
}
