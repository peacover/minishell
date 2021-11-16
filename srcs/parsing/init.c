/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <yer-raki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 12:27:06 by yer-raki          #+#    #+#             */
/*   Updated: 2021/11/16 12:27:54 by yer-raki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	init_t_sep(t_sep *node)
{
	node->path = NULL;
	node->t_sp = '\0';
	node->args = NULL;
	node->is_builtin = 0;
	node->builtin = NULL;
	node->upper_builtin = NULL;
	node->lower_builtin = NULL;
	node->is_red = 0;
	node->s_red = NULL;
	node->r_args = NULL;
	node->is_path_in_arg = 0;
	node->red = NULL;
}

void	init_data(t_vars *vars)
{
	vars->i = 0;
	vars->end = 0;
	vars->start = 0;
	vars->s1 = NULL;
	vars->ret = NULL;
	vars->is_dollar = 0;
	vars->v = NULL;
	vars->w = NULL;
}

void	init_node_red(t_red *red)
{
	red->r_file = NULL;
	red->red_op = '\0';
}
