/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/13 00:04:46 by yer-raki          #+#    #+#             */
/*   Updated: 2019/10/17 21:42:34 by yer-raki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void*ft_memchr(const void *s, int c, size_t n)
{
	char *str;

	str = (char *)s;
	while (n > 0)
	{
		if (*str == c)
			return (str);
		str++;
		n--;
	}
	return (NULL);
}
