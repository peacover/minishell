#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
# include <stdio.h>


//         int fd[2];
//         int fd1[2];
//         pipe(fd);
//         int pid = fork();
//         if (pid == 0)
//         {
//                 dup2(fd[1],1);
//                 close(fd[1]);
//                 close(fd[0]);
//                 execve(cmd[0],cmd,NULL);
//                 exit(0);
//         }
//         close(fd[1]);
//         // 2
//         pipe(fd1);
//         int pid2 = fork();
//         if (pid2 == 0)
//         {
//                 // char s[2] = {"/bin/cat", NULL};
//                 dup2(fd[0],0);
//                 dup2(fd1[1], 1);
//                 close(fd1[1]);
//                 close(fd[0]);
//                 close(fd1[0]);
//                 execve(cmd2[0],cmd2, NULL);
//                 exit(0);
//         }
//         close(fd[0]);
//         close(fd1[1]);
//         if (fork() == 0)
//         {
//                 dup2(fd1[0], 0);
//                 close(fd1[0]);
//                 execve(cmd3[0],cmd3, NULL);
//                 exit(0);
//         }
//         close(fd1[0]); 
//        wait(NULL);
//         wait(NULL);
//         wait(NULL);
    // return (0);

typedef struct         s_pipe
{
        char            **cmd;
        int             input;
        int             output;
}                       t_pipe;


void    init_pipes(t_pipe *s_pipe, char *cmd[3][3])
{
        int fd[2];
        for (int i = 0; i < 3; i++)
        {
                s_pipe[i].output = 1;
                s_pipe[i].input = 0;
        }
        for (int i  = 0; i < 3; i++)
        {
                s_pipe[i].cmd = cmd[i];
                if (i < 2)
                {
                        pipe(fd);
                        s_pipe[i + 1].input = fd[0];
                        s_pipe[i].output = fd[1];
                }
        }
}

void            ft_pipe(t_pipe *s_pipe)
{
        for (int i = 0; i < 3; i++)
        {
                int pid = fork();
                if (pid == 0)
                {
                        if (s_pipe[i].output != 1)
                        {
                                dup2(s_pipe[i].output, 1);
                                close(s_pipe[i].output);
                        }
                        if (s_pipe[i].input != 0)
                        {
                                dup2(s_pipe[i].input, 0);
                                close(s_pipe[i].input);
                        }
                        for (int j = 0; j < 3; j++)
                        {
                                if (s_pipe[j].output != 1)
                                        close(s_pipe[j].output);
                                if (s_pipe[j].input != 0)
                                        close(s_pipe[j].input);
                        }
                        execve(s_pipe[i].cmd[0], s_pipe[i].cmd, NULL);
                        exit(1);
                }
                if (s_pipe[i].output != 1)
                        close(s_pipe[i].output);
                if (s_pipe[i].input != 0)
                        close(s_pipe[i].input);
        }
        for (int i = 0; i < 3; i++)
        {
                wait(NULL);
        }
}
// int main(int argc,char **argv, char **env)
// {

//         t_pipe   p[3];
//         char *cmd[3][3] =
//         {
//                 {"/bin/echo", "hello hello", NULL},      
//                 {"/usr/bin/grep", "hello", NULL},
//                 {"/usr/bin/wc", "-c", NULL},
//         };
//         init_pipes(p, cmd);
//         ft_pipe(p);
//         return (0);
// }