/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   putenv.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <yer-raki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 15:42:00 by mhaddi            #+#    #+#             */
/*   Updated: 2021/11/17 09:55:46 by yer-raki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

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
