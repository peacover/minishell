/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <yer-raki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 16:08:10 by yer-raki          #+#    #+#             */
/*   Updated: 2021/04/07 11:17:56 by yer-raki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int     main(int argc, char **argv, char **env)
{
 
    t_infos infos;
    int i;
    char *str;
    char c;
    char *arg;
    int ret;
    
    while (1)
    {
        arg = malloc(1);
        i = 0;
        ft_putstr(getcwd(str, sizeof(str)));
        ft_putstr("> ");
        while (read(0, &c, 1) > 0)
        {
            if (c == '\n')
                break;
            arg = ft_realloc(arg, i, i + 1);
            arg[i] = c;
            i++;
        }
        arg[i] = '\0';
        printf("\narg : \"%s\" | len : %zu \n", arg, ft_strlen(arg));
        
        free(arg);
    }


    // printf ("arg2 : %s\n", ft_strjoin("test1", "test2"));
    // printf ("nb arg : %d", ft_strlen2(argv));
}