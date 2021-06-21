/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/22 10:08:31 by yer-raki          #+#    #+#             */
/*   Updated: 2019/11/03 16:39:02 by yer-raki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *str, const char *to_find, size_t len)
{
	char		*s;
	size_t		lto_find;

	lto_find = ft_strlen(to_find);
	s = (char *)str;
	if (lto_find == 0 || s == to_find)
		return (s);
	if (ft_strlen(str) < lto_find)
		return (NULL);
	while (*s && len-- >= lto_find)
	{
		if (ft_memcmp(s, to_find, lto_find) == 0)
			return (s);
		s++;
	}
	return (NULL);
}
