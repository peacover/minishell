/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 19:50:20 by yer-raki          #+#    #+#             */
/*   Updated: 2019/10/24 23:44:56 by yer-raki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	int		i;
	int		l;
	char	*d;

	i = 0;
	l = ft_strlen(s1);
	d = (char *)malloc(sizeof(char) * (l + 1));
	if (!d)
		return (NULL);
	while (l > i)
	{
		d[i] = s1[i];
		i++;
	}
	d[i] = '\0';
	return (d);
}
