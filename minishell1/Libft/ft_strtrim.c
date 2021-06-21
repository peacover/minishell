/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/14 17:35:13 by yer-raki          #+#    #+#             */
/*   Updated: 2019/10/25 22:52:14 by yer-raki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		positionl(char const *s1, char const *set)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (set[j] != '\0')
	{
		if (set[j] == s1[i])
		{
			i++;
			j = 0;
		}
		else
			j++;
	}
	return (i);
}

int		positionr(char const *s1, char const *set)
{
	size_t	i;
	size_t	t;

	i = 0;
	t = ft_strlen(s1);
	while (set[i] != '\0' && t > 0)
	{
		if (set[i] == s1[t - 1])
		{
			t--;
			i = 0;
		}
		else
			i++;
	}
	return (t);
}

char	*ft_strtrim(char const *s1, const char *set)
{
	size_t	i;
	size_t	t;
	size_t	j;
	char	*s2;

	if (!s1)
		return (NULL);
	if (!set)
		return (ft_strdup(s1));
	j = 0;
	i = positionl(s1, set);
	t = positionr(s1, set);
	if (i > t)
		return (ft_strdup("\0"));
	s2 = (char *)malloc((t - i + 1) * sizeof(char));
	if (!s2)
		return (NULL);
	while (t > i)
		s2[j++] = s1[i++];
	s2[j] = '\0';
	return (s2);
}
