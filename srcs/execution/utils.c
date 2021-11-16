/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <yer-raki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 15:52:34 by mhaddi            #+#    #+#             */
/*   Updated: 2021/11/16 18:16:09 by yer-raki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
