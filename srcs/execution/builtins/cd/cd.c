/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 15:22:59 by mhaddi            #+#    #+#             */
/*   Updated: 2021/11/16 15:36:54 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

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
