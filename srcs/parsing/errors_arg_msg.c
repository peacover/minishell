/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_arg_msg.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <yer-raki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 12:04:32 by yer-raki          #+#    #+#             */
/*   Updated: 2021/11/16 12:05:48 by yer-raki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	error_msg(char *s)
{
	ft_putstr(s);
	ft_putchar('\n');
	exit(0);
}

int	handling_errors_arg(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (1);
	while (str[i])
	{
		if (str[i] == ';' && str[i + 1] == ';')
			error_msg("syntax error near unexpected token `;;'");
		else if (str[i] == '|' && str[i + 1] == '|')
			error_msg("syntax error near unexpected token `||'");
		else if (str[i] == ';' && str[i + 1] == '|')
			error_msg("syntax error near unexpected token `;'");
		else if (str[i] == '|' && str[i + 1] == ';')
			error_msg("syntax error near unexpected token `|'");
		if ((str[i] == ';' || str[i] == '|')
			&& (str[i + 1] == ';' || str[i + 1] == '|'))
			return (1);
		i++;
	}
	return (0);
}
