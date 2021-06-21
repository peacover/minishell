#include "../minishell.h"
#include <dirent.h>

void    ft_cd(char **args)
{

    char *r;
    printf("%s\n",getcwd(r,1));
    if(chdir(args[1]) != 0)
        printf("Error\n");
    // chdir(args[1]);
    printf("%s\n",getcwd(r,1));
}

// int main(int argc, char **argv)
// {
//     ft_cd(argv);

//     return 0;
// }