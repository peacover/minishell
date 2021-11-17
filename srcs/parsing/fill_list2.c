/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_list2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <yer-raki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 07:59:42 by yer-raki          #+#    #+#             */
/*   Updated: 2021/11/17 07:59:48 by yer-raki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	get_builtin(char *s, t_sep *node)
{
	int		i;
	int		l;

	i = 0;
	l = ft_strlen(s);
	if (s[i] == '|')
	{
		i++;
		while (s[i] && s[i] == ' ')
			i++;
		s = ft_substr(s, i, l - i);
		add_garbage((void *)&s);
	}
	i = 0;
	while (s[i] && s[i] == ' ')
		i++;
	if (!check_red(node, s))
		get_args(s, i, node);
}
