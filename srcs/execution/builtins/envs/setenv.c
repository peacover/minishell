/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setenv.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 15:33:24 by mhaddi            #+#    #+#             */
/*   Updated: 2021/11/16 16:34:54 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

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
