/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <yer-raki@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/10 23:56:25 by yer-raki          #+#    #+#             */
/*   Updated: 2021/07/03 20:45:17 by yer-raki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <string.h>

char	*ft_strrchr(const char *s, int c)
{
	int		t;
	char	*dst;

	t = 0;
	dst = (char *)s;
	while (dst[t] != '\0')
		t++;
	while (t >= 0)
	{
		if (s[t] == (char)c)
			return ((char *)dst + t);
		t--;
	}
	return (0);
}
