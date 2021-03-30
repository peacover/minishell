/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/12 22:24:19 by yer-raki          #+#    #+#             */
/*   Updated: 2019/10/24 22:09:03 by yer-raki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char	*s;
	char	*d;
	size_t	i;

	i = 0;
	s = (char *)src;
	d = (char *)dst;
	if (!s && !d)
		return ("NULL");
	if (d <= s)
	{
		while (len-- > 0)
			*d++ = *s++;
	}
	else
	{
		while (len >= ++i)
			d[len - i] = s[len - i];
	}
	return (dst);
}
