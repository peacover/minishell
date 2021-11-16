/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_list_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <yer-raki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 12:09:36 by yer-raki          #+#    #+#             */
/*   Updated: 2021/11/16 12:10:05 by yer-raki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	fill_env2(t_env	**temp, char **env, int i)
{
	char	**s;

	s = ft_split(env[i], '=');
	*temp = malloc(sizeof(**temp));
	add_garbage((void *)temp);
	(*temp)->val = ft_strdup(env[i]);
	add_garbage((void *)&((*temp)->val));
	(*temp)->key = ft_strdup(s[0]);
	add_garbage((void *)&((*temp)->key));
	(*temp)->value = ft_strdup(s[1]);
	add_garbage((void *)&((*temp)->value));
	(*temp)->next = NULL;
	free_t2(s);
	add_garbage((void *)temp);
}

void	fill_env3(t_env **temp)
{
	*temp = malloc(sizeof(**temp));
	add_garbage((void *)temp);
	(*temp)->val = ft_strdup("?=0");
	add_garbage((void *)&((*temp)->val));
	(*temp)->key = ft_strdup("?");
	add_garbage((void *)&((*temp)->key));
	(*temp)->value = ft_strdup("0");
	add_garbage((void *)&((*temp)->value));
	(*temp)->next = NULL;
	add_garbage((void *)temp);
}

t_env	*fill_env(char **env)
{
	t_env	*head;
	t_env	*last;
	t_env	*temp;
	int		i;

	i = 0;
	head = NULL;
	while (env[i])
	{
		fill_env2(&temp, env, i);
		if (!head)
		{
			head = temp;
			last = temp;
		}
		else
		{
			last->next = temp;
			last = last->next;
		}
		i++;
	}
	fill_env3(&temp);
	last->next = temp;
	return (head);
}
