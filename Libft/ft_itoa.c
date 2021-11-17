/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <yer-raki@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/15 21:34:23 by yer-raki          #+#    #+#             */
/*   Updated: 2021/07/03 20:42:13 by yer-raki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	len_int(int n)
{
	int				l;
	unsigned int	nb;

	l = 1;
	if (n < 0)
	{
		l++;
		nb = (unsigned int)(-n);
	}
	else
		nb = (unsigned int)n;
	while (nb > 9)
	{
		nb /= 10;
		l++;
	}
	return (l);
}

char	*ft_itoa(int n)
{
	int				l;
	char			*s;
	unsigned int	nb;

	l = len_int(n);
	s = (char *)malloc(sizeof(char) * (l + 1));
	if (!s)
		return (NULL);
	s[l] = '\0';
	l--;
	if (n >= 0)
		nb = (unsigned int)n;
	else
		nb = (unsigned int)(-n);
	while (l >= 0)
	{
		s[l] = (nb % 10) + '0';
		nb /= 10;
		l--;
	}
	if (n < 0)
		s[0] = '-';
	return (s);
}
