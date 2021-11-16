/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <yer-raki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/02 13:28:12 by yer-raki          #+#    #+#             */
/*   Updated: 2021/11/16 18:49:12 by yer-raki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*red_redim_s(char *s, int start, int end)
{
	char	*s1;
	char	*s2;

	s1 = ft_substr(s, 0, start);
	s2 = ft_substr(s, end + 1, ft_strlen(s) - end + 1);
	s = ft_strjoin(s1, " ");
	add_garbage((void *)&s);
	s = ft_strjoin(s, s2);
	add_garbage((void *)&s);
	free(s1);
	free(s2);
	return (s);
}

char	*str_upper(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		s[i] = ft_toupper(s[i]);
		i++;
	}
	return (s);
}

char	*str_lower(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		s[i] = ft_tolower(s[i]);
		i++;
	}
	return (s);
}

int	search_sq(char *s, int start, char type)
{
	if (type == '\'')
	{
		while (s[start])
		{
			if (s[start] == type)
				return (start);
			start++;
		}
		return (0);
	}
	else if (type == '\"')
	{
		while (s[start])
		{
			if (s[start] == type && s[start] != '\\')
				return (start);
			start++;
		}
		return (0);
	}
	return (0);
}

char	*remove_char(char *s, int i)
{
	char	*s1;
	char	*s2;
	int		l;

	l = (int)ft_strlen(s);
	s1 = ft_substr(s, 0, i);
	s2 = ft_substr(s, i + 1, l - i + 1);
	free(s);
	s = ft_strjoin(s1, s2);
	add_garbage((void *)&s);
	free(s1);
	free(s2);
	return (s);
}

char	*handling_bs_dq(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\\')
		{
			if (!s[i + 1])
			{
				error_msg("error multiligne");
				break ;
			}
			if (s[i + 1] == '\\')
				s = remove_char(s, i);
		}
		i++;
	}
	return (s);
}

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
	if (!w)
		return (0);
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
	
	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	new = malloc(new_size * sizeof(char));
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

char	**ft_realloc_2(char **old, size_t old_size, size_t new_size)
{
	char **new;
	int		i;
	
	new = malloc(sizeof(char *) * (new_size + 1));
	add_garbage((void *)&new);
	i = 0;
	while (i < (int)old_size)
	{
		new[i] = old[i];
		i++;
	}
	return new;
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