#include "../minishell.h"

typedef struct node {
    char *val;
    char *value;
    char *key;
    struct node * next;
} node_t;

void print_list(node_t *head)
{
    node_t * current = head;
    while(current != NULL)
    {
        printf("declare -x ");
        printf("%s",current->key);
        printf("=\"");
        printf("%s\"\n",current->value);
        current = current->next;
    }
}

void       delet_v_env(node_t **head,node_t *tail,char *argv,int argc)
{
    node_t *current = *head;
    node_t *temp, *prev;
    char **s = ft_split(argv,'=');
    if (strcmp(current->key, s[0]) == 0 && current != NULL)
    {
        temp = current->next;
        free(current);
        current = NULL;
        *head = temp;
        write(1, "--\n", 3);
        return ;
    }
    while(current != NULL)
    {
        if(strcmp(current->key,s[0]) == 0)
        {
            temp = current->next;
            prev->next = temp;
            free(current);
            break ;
        }
        prev = current;
        current = current->next;
    }
}

void    ft_unset(int argc,char **argv, char **env)
{
    node_t *head = NULL;
    node_t *tail = NULL;
    int i;
    i = 0;
    node_t *temp ;
    char **s;
    while (env[i])
    {
        s = ft_split(env[i],'=');
        temp = malloc(sizeof(node_t *));
        temp->val = strdup(env[i]);
        temp->key = s[0];
        temp->value= s[1];
        temp->next = NULL;
        if (!head)
        {
            head = temp;
            tail = head;
        }
        else
        {
            tail->next = temp;
            tail = temp;
        }
        i++;
    }
    i = 0;
    if (!argv[1])
        printf("Error: No Args");
    if (argv[1])
    {
        while (argv[++i])
        {
            delet_v_env(&head, tail,argv[i],argc);
        }
    }
    print_list(head);
}