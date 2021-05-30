/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <yer-raki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/02 13:28:12 by yer-raki          #+#    #+#             */
/*   Updated: 2021/05/30 18:11:26 by yer-raki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		ft_strcmp(char *s1, char *s2)
{
	int i;

	i = 0;

	if (!s1)
		return (s2[i]);
	else if (!s2)
		return (s1[i]);
	while (s1[i] && s2[i] && s1[i] == s2[i])
	{
		i++;
	}
	return (s1[i] - s2[i]);
}

int		ft_strlen2(char **w)
{
	int i;

	i = 0;
	while (w[i])
		i++;
	return (i);
}

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putstr(char *s)
{
	int i;

	i = 0;
	while (s[i])
	{
		ft_putchar(s[i]);
		i++;
	}
}

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new;
	// char	*p;

	// p = (char *)ptr;
	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	new = malloc(new_size * sizeof(char*));
	if (!new)
		return (NULL);
	if (ptr != NULL || !old_size)
	{
		if (old_size >= new_size)
			ft_memcpy(new, ptr, new_size);
		else
			ft_memcpy(new, ptr, old_size);
		free(ptr);
	}
	return (new);
}

void	ft_putnbr(int n)
{
	unsigned int nb;

	if (n < 0)
	{
		ft_putchar('-');
		n = -n;
	}
	nb = n;
	if (nb > 9)
	{
		ft_putnbr(nb / 10);
		ft_putchar((nb % 10) + '0');
	}
	else
		ft_putchar(nb + '0');
}

// int		nb_w(char const *s)
// {
// 	int i;
// 	int x;
// 	int l;

// 	i = 0;
// 	x = 0;
// 	l = 0;
// 	while (s[i] != '\0')
// 	{
// 		if (s[i] == ' ' || s[i] == ';' || s[i] == '|')
// 			x = 0;
// 		else if (x == 0)
// 		{
// 			x = 1;
// 			l++;
// 		}
// 		i++;
// 	}
// 	return (l);
// }

// int		nb_c(char const *s, int i)
// {
// 	int l;

// 	l = 0;
// 	while (s[i] != ' ' && s[i] != ';' && s[i] != '|' && s[i] != '\0')
// 	{
// 		l++;
// 		i++;
// 	}
// 	return (l);
// }

// char	**ft_split_edited(char const *s)
// {
// 	char	**w;
// 	int		i;
// 	int		j;
// 	int		k;

// 	i = 0;
// 	j = 0;
// 	if (!(w = (char**)malloc(sizeof(char *) * (nb_w(s) + 1))) || !s)
// 		return (NULL);
// 	while (s[i] != '\0' && nb_w(s) > j)
// 	{
// 		k = 0;
// 		while (s[i] == ' ' || s[i] == ';' || s[i] == '|')
// 			i++;
// 		w[j] = (char *)malloc(sizeof(char) * (nb_c(s, i) + 1));
// 		while (s[i] != ' ' && s[i] != ';' && s[i] != '|' && s[i])
// 			w[j][k++] = s[i++];
// 		w[j][k] = '\0';
// 		j++;
// 	}
// 	w[j] = 0;
// 	return (w);
// }

void	free_t2(char **w)
{
	int i;
	int	x;

	i = 0;
	x = ft_strlen2(w);
	while (x > i)
	{
		free(w[i]);
		i++;
	}
	free(w);
}

char	*ft_strcpy(char *dest, char *src)
{
	int i;

	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}