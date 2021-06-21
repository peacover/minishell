#include "../minishell.h"

void ft_echo(int argc, char **args)
{
    int i;
    int check_n;
    int sp = 0;

    check_n = 0;
    if (!args[1])
        write(1,"\n",1);
    i = 2;
    if(args[1] && args[i][0] == '-' && args[i][1] == 'n' && args[i][2] == '\0')
    {
        i++;
        sp = 1;
    }
    while(i < argc)
    {
        printf("%s",args[i]);
        i++;
        if(args[i])
            printf(" ");
    }
    if(sp == 0)
        printf("\n");
}
