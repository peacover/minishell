/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <yer-raki@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/23 18:58:27 by yer-raki          #+#    #+#             */
/*   Updated: 2021/07/03 20:48:36 by yer-raki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	r;

	r = (char)c;
	while (*s != '\0')
	{
		if (*s == r)
			return ((char *)s);
		s++;
	}
	if (*s == r)
		return ((char *)s);
	return (NULL);
}
