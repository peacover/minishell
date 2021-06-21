#include "../minishell.h"

typedef struct s_envp {
    char *val;
    struct s_envp * next;
} t_envp;

t_envp g_env;

void  ft_env(char **env)
{
    t_envp *envp = NULL;
    t_envp *head = NULL;
    t_envp *temp;

    int i;
    i = -1;
    while (env[++i])
    {
        temp = malloc(sizeof(envp));
        temp->val = env[i];
        temp->next = NULL;
        if (!head)
        {
            head = temp;
            envp = head;
        }
        else
        {
            envp->next = temp;
            envp = temp;
        }
    }
     t_envp  *crr = head;
    while(crr != NULL)
    {
        printf("%s\n",crr->val);
        crr = crr->next;
    }
}

// int main(int argc,char **argv,char **env)
// {
//     ft_env(env);
//     return 0;
// }