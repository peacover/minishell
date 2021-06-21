
#include "../minishell.h"


void	ft_putchar(char c)
{
	write(1,&c,1);
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

int main(int argc,char **argv,char **env)
{
    int i;
    char *str;
    char c;

	while (1)
    {
		str = NULL;
        i = 0;
        ft_putstr("> ");
        while (read(0, &c, 1) > 0)
        {
            if (c == '\n')
                break;
            str = ft_realloc(str, i, i + 2);
            str[i] = c;
            i++;
			str[i] = '\0';
        }
		ft_checkcmd(str,env);
		// printf("%s\n",str);
		// ft_checkcmd(argc,argv,env);
    }
    return (0);
}