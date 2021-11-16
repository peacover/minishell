/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 15:47:35 by mhaddi            #+#    #+#             */
/*   Updated: 2021/11/16 16:34:44 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

int	check_key_syntax(int condition, char *key)
{
	if (condition)
	{
		printf("minishell: unset: `%s': not a valid identifier\n", key);
		return (1);
	}
	return (0);
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
