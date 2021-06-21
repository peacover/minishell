# include <string.h>
# include <unistd.h>
# include <stdlib.h>

typedef struct	s_split_next
{
	size_t start;
	size_t length;
}				t_split_next;

size_t	ft_strlen(const char *str)
{
	size_t i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char		*res;
	size_t		i;

	if (!s)
		return (0);
	if (start > ft_strlen(s))
	{
		res = malloc(sizeof(char) * 1);
		res[0] = '\0';
		return (res);
	}
	i = 0;
	if (!(res = malloc(len + 1)))
		return (NULL);
	while (i < len)
		res[i++] = s[start++];
	res[i] = '\0';
	return (res);
}

static char
	**ft_alloc_split(char const *s, char c)
{
	size_t	i;
	char	**split;
	size_t	total;

	i = 0;
	total = 0;
	while (s[i])
	{
		if (s[i] == c)
			total++;
		i++;
	}
	split = (char**)malloc(sizeof(s) * (total + 2));
	if (!split)
		return (NULL);
	return (split);
}

void
	*ft_free_all_split_alloc(char **split, size_t elts)
{
	size_t	i;

	i = 0;
	while (i < elts)
	{
		free(split[i]);
		i++;
	}
	free(split);
	return (NULL);
}

static void
	*ft_split_range(char **split, char const *s,
		t_split_next *st, t_split_next *lt)
{
	split[lt->length] = ft_substr(s, st->start, st->length);
	if (!split[lt->length])
		return (ft_free_all_split_alloc(split, lt->length));
	lt->length++;
	return (split);
}

static void
	*ft_split_by_char(char **split, char const *s, char c)
{
	size_t			i;
	t_split_next	st;
	t_split_next	lt;

	i = 0;
	lt.length = 0;
	lt.start = 0;
	while (s[i])
	{
		if (s[i] == c)
		{
			st.start = lt.start;
			st.length = (i - lt.start);
			if (i > lt.start && !ft_split_range(split, s, &st, &lt))
				return (NULL);
			lt.start = i + 1;
		}
		i++;
	}
	st.start = lt.start;
	st.length = (i - lt.start);
	if (i > lt.start && i > 0 && !ft_split_range(split, s, &st, &lt))
		return (NULL);
	split[lt.length] = 0;
	return (split);
}

char
	**ft_split(char const *s, char c)
{
	char	**split;

	if (!(split = ft_alloc_split(s, c)))
		return (NULL);
	if (!ft_split_by_char(split, s, c))
		return (NULL);
	return (split);
}