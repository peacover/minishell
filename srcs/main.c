/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <yer-raki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 16:08:10 by yer-raki          #+#    #+#             */
/*   Updated: 2021/11/16 18:22:37 by yer-raki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

/*void	print_red(t_red *red)
{
	int i;

	i = 0;
	while (red)
	{
		printf(" \nRED_Node: %d\n", i);
		printf("\n---------------------------\n");
		printf (" File : %s", red->r_file);
		printf ("\n Type_red : %c", red->red_op);
		printf("\n---------------------------\n");
		red = red->next;
		i++;
	}
}

void    print_mylist(t_sep *node, int pipes_num)
{
	int i;
	int l;
	int l2;
	int node_num;

	i = 0;
	node_num = 0;
	while (node != NULL)
	{
		printf(" \nNode: %d\n", node_num);
		printf("\n------------------------------------------\n");
		printf (" path : %s", node->path);
		printf ("\n cmd : %s", node->builtin);
		printf ("\n is_builtin : %d", node->is_builtin);
		printf ("\n builtin : %s", node->builtin);
		printf ("\n s_red : %s", node->s_red);
		printf ("\n sep : %c", node->t_sp);
		printf ("\n pipes_num : %d", pipes_num);

		printf ("\n\n is_red : %d", node->is_red);
		print_red(node->red);
		i = 0; 
		l = ft_strlen2(node->args);
		l2 = ft_strlen2(node->r_args);
		while (l > i)
		{
			printf ("\n arg %d : |%s|", i, node->args[i]);
			i++;
		}
		// while (l2 > i)
		// {
		// 	printf ("\n r_arg %d : |%s|", i, node->r_args[i]);
		// 	i++;
		// }
		printf("\n------------------------------------------\n\n");
		node_num++;
		node = node->next;
	}
}*/

void	red_add_to_args(int start, int end, int *i, t_sep *node)
{
	int		l;
	char	*str;
	char	*s;
	int		j;

	l = 0;
	s = node->red_args;
	add_to_args2(*i, &j, node, &str);
	while (s[start] && start < end)
	{
		add_to_args4(s, &l, &str, start);
		if (*i == 0)
			return (add_to_args3(node, str, i));
		else
		{
			add_to_args5(node, &str, j, i);
			if (s[start] == '\'' || s[start] == '\"')
				start = l + 1;
			else
				start = l;
		}
	}
	if (node->args[j])
		node->args[j + 1] = NULL;
}

char	*red_get_file2(t_sep *node, int i, int start, char *s)
{
	char	*file;
	int		end;

	file = NULL;
	end = search_sq(s, start + 1, s[start]);
	if (!end)
		error_msg("error multiligne");
	file = ft_substr(s, start + 1, end - start - 1);
	add_garbage((void *)&file);
	if (s[start] != '\'')
	{
		if (s[start] == '\"')
			file = handling_bs_dq(file);
		file = handling_dollar(file);
	}
	node->red_args = red_redim_s(s, i, end);
	return (file);
}

char	*red_get_file3(t_sep *node, int i, int start, char *s)
{
	char	*file;
	int		end;

	file = NULL;
	end = start;
	while (s[end] && s[end] != ' ' && s[end] != '<' && s[end] != '>')
		(end)++;
	file = ft_substr(s, start, end - start);
	add_garbage((void *)&file);
	node->red_args = red_redim_s(s, i, end - 1);
	if (s[start] != '\'')
	{
		if (s[start] == '\"')
			file = handling_bs_dq(file);
		file = handling_dollar(file);
	}
	return (file);
}

char	*red_get_file(t_sep *node, char *s, int start, char type)
{
	int	i;

	i = 0;
	while (s[start])
	{
		node->red_args = NULL;
		i = start;
		if (type == 'i' || type == 'o' )
			start++;
		else if (type == 'a' || type == 'h')
			start += 2;
		while (s[start] && s[start] == ' ')
			start++;
		if (s[start] == '\'' || s[start] == '\"')
		{
			if (i != 0 && s[start - 1] == '\\')
				continue ;
			return (red_get_file2(node, i, start, s));
		}
		else
			return (red_get_file3(node, i, start, s));
	}
	return (NULL);
}

void	red_get_cmd_args2(t_sep *node, int *start, int *end, int *i)
{
	*end = *start;
	while (node->red_args[*end] && node->red_args[*end] != ' ')
		(*end)++;
	if (*end > *start)
		red_add_to_args(*start, *end, i, node);
	*start = *end;
}

void	red_get_cmd_args(t_sep *node)
{
	int		i;
	int		end;
	int		start;

	i = 0;
	start = 0;
	while (node->red_args[start])
	{
		if (node->red_args[start] == '\'' || node->red_args[start] == '\"')
		{
			if (node->red_args[start - 1] == '\\')
				continue ;
			end = search_sq(node->red_args, start + 1, node->red_args[start]);
			if (!end)
				error_msg("error multiligne");
			red_add_to_args(start, end, &i, node);
			start = end + 1;
		}
		else
			red_get_cmd_args2(node, &start, &end, &i);
		while (node->red_args[start] && node->red_args[start] == ' ')
			start++;
	}
}

char	red_get_type(char *s, int start)
{
	while (s[start])
	{
		if (s[start] == '>')
		{
			if (s[start + 1] == '>')
			{
				if (s[start + 2] == '>')
					error_msg("syntax error near unexpected token `>'");
				return ('a');
			}
			return ('o');
		}
		if (s[start] == '<')
		{
			if (s[start + 1] == '<')
			{
				if (s[start + 2] == '<')
					error_msg("syntax error near unexpected token `<'");
				return ('h');
			}
			return ('i');
		}
		start++;
	}
	return ('\0');
}

void	fill_node_red(t_red *red, char *file, char type)
{
	red->r_file = file;
	red->red_op = type;
}

void	fill_red_list(t_red **head, char *file, char type)
{
	int		i;
	t_red	*newnode;
	t_red	*lastnode;

	i = 0;
	newnode = malloc(sizeof(t_red));
	add_garbage((void *)&newnode);
	lastnode = *head;
	init_node_red(newnode);
	fill_node_red(newnode, file, type);
	newnode->next = NULL;
	if (*head == NULL)
		*head = newnode;
	else
	{
		lastnode = *head;
		while (lastnode->next)
			lastnode = lastnode->next;
		lastnode->next = newnode;
	}
}

void	red_get_type_file2(int *i, int *end, char *s)
{
	while (s[*i] && s[*i] == ' ')
		(*i)++;
	if (s[*i] == '\'' || s[*i] == '\"')
	{
		*end = search_sq(s, *i + 1, s[*i]);
		if (!*end)
			error_msg("error multiligne");
		*i = *end;
	}
}

void	red_get_type_file(t_sep *node, char *s)
{
	int		i;
	char	type;
	char	*file;
	int		end;

	i = 0;
	file = NULL;
	end = 0;
	node->red_args = NULL;
	while (s[i])
	{
		red_get_type_file2(&i, &end, s);
		if ((s[i] == '>' ) || (s[i] == '<' ))
		{
			type = red_get_type(s, i);
			file = red_get_file(node, s, i, type);
			s = node->red_args;
			fill_red_list(&node->red, file, type);
		}
		if ((int)ft_strlen(s) < i + 1)
			break ;
		i++;
	}
	red_get_cmd_args(node);
}

int	check_red(t_sep *node, char *s)
{
	int	start;
	int	end;

	start = 0;
	while (s[start])
	{
		if (s[start] == '\'' || s[start] == '\"')
		{
			end = search_sq(s, start + 1, s[start]);
			if (!end)
				error_msg("error multiligne");
			start = end;
		}
		else if ((start == 0 && (s[start] == '>' || s[start] == '<'))
			|| (start > 0 && (s[start] == '>' || s[start] == '<')
				&& s[start - 1] != '\\'))
		{
			node->is_red = 1;
			red_get_type_file(node, s);
			return (1);
		}
		start++;
	}
	return (0);
}

void	ignctl(void)
{
	struct termios	term;

	if (tcgetattr(STDIN_FILENO, &term) != 0)
		perror("tcgetattr() error");
	term.c_lflag &= ~ECHOCTL;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &term) != 0)
		perror("tcsetattr() error");
}

int	main(int argc, char **argv, char **env)
{
	int		i;
	char	*str;
	int		ret;

	(void)argc;
	(void)argv;
	g_data.gg = NULL;
	g_data.envp = env;
	g_data.is_forked = 0;
	g_data.envl = fill_env(env);
	i = 0;
	ret = 0;
	ignctl();
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, signal_handler_parent);
	while (1)
	{
		str = NULL;
		str = readline("\x1B[32m$> \e[0m");
		if (!str)
		{
			//if (isatty(0))
				write(2, "\b\bexit\n", 7); // return 1
			break ;
		}
		if (str[0] == '\0')
		{
			free(str);
			continue ;
		}
		add_history(str);
		if (!handling_errors_arg(str))
		{
			if (fill_list(str) == 1)
			{
				free(str);
				free_garbage();
				return (1);
			}
		}
		free(str);
	}
	free_garbage();
	return (0);
}