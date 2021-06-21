#include "../minishell.h"

typedef struct node {
    char *val;
    char *value;
    char *key;
    struct node * next;
} node_t;

void print_list1(node_t *head)
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

void    add_list(node_t *head,node_t *tail,char *argv,int argc)
{
    node_t *current = head;
    int b = 0;
    int i = 1;
    char **s = ft_split(argv,'=');
    while(current != NULL)
    {
        if(strcmp(current->key,s[0]) == 0)
        {
            current->value = s[1];
            b=1;
            break;
        }
            current = current->next;
    }
    if(!b)
        {
            node_t *temp = malloc(sizeof(*temp));
            temp->key = s[0];
            temp->value= s[1];
            temp->next = NULL;
            tail->next = temp;
        }
}

void    ft_export(int argc,char **argv, char **env)
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
        temp = malloc(sizeof(*temp));
        temp->val = env[i];
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
    i = 1;
    if (argv[2])
    {
        while (argv[++i])
        {
            add_list(head, tail,argv[i],argc);
        }
    }
    print_list1(head);
}