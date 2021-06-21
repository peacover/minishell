#include <unistd.h>
#include <stdio.h>
#include<sys/wait.h>


// int main(int argc,char **args,char **env)
// {
//     int id = fork();
//     if (id == 0)
//         execve("/bin/cat",args,env);
//     if (id){
//         wait(NULL);
//         printf("\n");
//         printf("amine\n");
//     }
//     return (0);
// }