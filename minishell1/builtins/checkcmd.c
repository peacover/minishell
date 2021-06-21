#include "../minishell.h"


void    ft_checkcmd(char *argv,char **env)
{
    if(strcmp("echo",argv[1]) == 0)
        ft_echo(argc,argv);
    if(strcmp("env",argv) == 0)
        ft_env(env);
    if(strcmp("export",argv[1]) == 0)
        ft_export(argc,argv,env);
    if(strcmp("unset",argv[1]) == 0)
        ft_unset(argc,argv,env);

}

// int main(int argc,char **argv,char **env)
// {
//     if (!argv[1])
//         return (0);
//     ft_checkcmd(argc,argv,env);
//     return (0);
// }