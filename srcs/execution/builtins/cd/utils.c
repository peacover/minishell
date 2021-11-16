/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 15:27:04 by mhaddi            #+#    #+#             */
/*   Updated: 2021/11/16 16:34:33 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

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

void	check_tilde(char **args)
{
	if (args && *args && **args == '~')
	{
		free(args[0]);
		args[0] = ft_getenv("HOME");
	}
}
