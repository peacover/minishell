#include "../minishell.h"

void ft_pwd()
{
    char *r = NULL;
    printf("%s\n",getcwd(r,1));
}