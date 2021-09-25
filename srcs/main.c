/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yer-raki <yer-raki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 16:08:10 by yer-raki          #+#    #+#             */
/*   Updated: 2021/09/25 16:46:49 by yer-raki         ###   ########.fr       */
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

void    error_msg(char *s)
{
	ft_putstr(s);
	ft_putchar('\n');
	exit(0);
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
		printf ("\n file : %s", node->r_file);
		printf ("\n type_red : %c", node->red_op);
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
}

char    *remove_char(char *s, int i)
{
	char    *s1;
	char    *s2;
	int		l;
	
	l = (int)ft_strlen(s);
	s1 = ft_substr(s, 0, i);
	s2 = ft_substr(s, i + 1, l - i + 1);
	free(s);
	s = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (s);
}

int    handling_errors_arg(char *str)
{
	int i;

	i = 0;
	if (!str)
		return (1);
	while (str[i])
	{
		if (str[i] == ';' && str[i + 1] == ';')
			error_msg("syntax error near unexpected token `;;'");
		else if (str[i] == '|' && str[i + 1] == '|')
			error_msg("syntax error near unexpected token `||'");
		else if (str[i] == ';' && str[i + 1] == '|')
			error_msg("syntax error near unexpected token `;'");
		else if (str[i] == '|' && str[i + 1] == ';')
			error_msg("syntax error near unexpected token `|'");
		if ((str[i] == ';' || str[i] == '|') && (str[i + 1] == ';' || str[i + 1] == '|'))
			return (1);
		i++;
	}
	return (0);
}

void	check_fill_path(t_sep *node)
{
	int     i;
	char    **w;
	char    *s;
	int     fd;
	
	i = 0;
	fd = 0;
	s = NULL;
	t_env *current = g_env;
	while (current != NULL)
	{
		if (!ft_strcmp(current->key, "PATH"))
		{
			w = ft_split(current->value, ':');
			while (w[i])
			{
				s = ft_strjoin(w[i], "/");
				s = ft_strjoin(s, node->lower_builtin);
				fd = open(s, O_RDONLY);
				if (fd > 0)
				{
					node->path = s;
					close(fd);
					return ;
				}
				close(fd);
				free(s);
				i++;
			}
		}
		current = current->next;
	}
	// printf (" \npath : %s \n", node->path);
	// free w
	node->path = NULL;
}

void	check_builtin(t_sep *node)
{
	int i;

	i = 0;
	if (!ft_strcmp(node->upper_builtin, "ECHO") || !ft_strcmp(node->upper_builtin, "CD")
	|| !ft_strcmp(node->upper_builtin, "PWD") || !ft_strcmp(node->upper_builtin, "EXPORT")
	|| !ft_strcmp(node->upper_builtin, "UNSET") || !ft_strcmp(node->upper_builtin, "ENV")
	|| !ft_strcmp(node->upper_builtin, "EXIT"))
	{
		node->is_builtin = 1;
	}
}

int     search_second_quote(char *s, int start, char type)
{
	if (type == '\'')
	{
		while (s[start])
		{
			if (s[start] == type)
				return (start);
			start++;
		}
		return (0);
	}
	else if (type == '\"')
	{
		while (s[start])
		{
			if (s[start] == type && s[start]!= '\\')
				return (start);
			start++;
		}
		return (0);
	}
	return (0);
}

char	*handling_bs(char *s)
{
	int i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\\')
		{
			if (!s[i + 1])
			{
				error_msg("error multiligne");
				break;
			}
			s = remove_char(s, i);
		}
		i++;
	}
	return (s);
}

char	*handling_bs_dq(char *s)
{
	int i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\\')
		{
			if (!s[i + 1])
			{
				error_msg("error multiligne");
				break;
			}
			if (s[i + 1] == '\\')
				s = remove_char(s, i);
		}
		i++;
	}
	return (s);
}

char	*replace_dollar(char *s, char *v, int start, char *key, int end)
{
	char *s1;
	char *s2;
	// char *s3;
	int l_key;
	int l_s;
	char *w;

	w = NULL;
	(void)end;
	return (v);
	l_key = ft_strlen(key);
	l_s = ft_strlen(s);
	// if (end)
	// 	s3 = ft_substr(s, end, l_s - end);
	s1 = ft_substr(s, 0, start - 1);
	s2 = ft_substr(s, start + l_key, l_s - start - l_key);
	w = ft_strjoin(s1, v);
	w = ft_strjoin(w, s2);
	return (w);
}
int		equal_export(char *s, int i)
{
	i++;
	while (s[i] && s[i] != '$' && (ft_isalpha(s[i]) || s[i] == '_' || ft_isdigit(s[i])))
		i++;
	return (i);
}

char	*str_export_split(char *s, int start, int is_dollar)
{
	int i;
	// int l;

	// i = end - 1;
	// l = 0;
	// if (count_dollar == 1)
	// 	return (ft_substr(s, 0, end));
	// else
	// {
	// 	while (i > 0 && s[i] != '$')
	// 	{
	// 		l++;
	// 		i--;
	// 	}
	// 	return (ft_substr(s, i, l + 1));
	// }
	i = start;
	if (is_dollar)
	{
		while (s[i] && s[i] != '$' && (ft_isalpha(s[i]) || s[i] == '_' || ft_isdigit(s[i])))
			i++;
		return (ft_substr(s, start, i - start));
	}
	else
	{
		while (s[i] && s[i] != '$')
			i++;
		return (ft_substr(s, start, i - start));
	}
}
int		check_dollar(char *s)
{
	int i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '$')
			return (1);
		i++;
	}
	return (0);
}
char	*handling_dollar(char *s, t_sep *node)
{
	int		i;
	int		end;
	int		start;
	char    *s1;
	char    *s2;
	char	*ret;
	int		count_dollar;
	int		is_dollar;
	char	*v;
	char	*w;
	t_env	*current = g_env;
   
	i = 0;
	start = 0;
	s1 = NULL;
	s2 = NULL;
	end = 0;
	w = NULL;
	count_dollar = 0;
	is_dollar = 0;
	(void)node;
	while (s[i] && check_dollar(s))
	{
		// start = 0;
		ret = NULL;
		if (ft_strlen(s) == 1 && s[0] == '$')
			return (ft_strdup("$"));
		if (s[i] == '$')
		{
			count_dollar++;
			is_dollar = 1;
			//node->is_builtin &&
			// if ( (s[equal_export(s, i)] == '=' || s[equal_export(s, i)] == '/'))
				end = equal_export(s, i);
			start = ++i;
			// while (s[i] && s[i] != ' ' && s[i] != '$')
			// 	i++;
			v = ft_substr(s, start, i - start);
			if (end)
				v = ft_substr(s, start, end - start);
			// printf ("\n s : |%s|\n", s);
			// while (s[i] && s[i] != '$')
			// 	i++;
			s1 = str_export_split(s, start, 1);
			while (current != NULL)
			{
				if (!ft_strcmp(current->key, v) || (!ft_strncmp(current->key, v, ft_strlen(current->key))
				))
				{
					// printf ("\n key : |%s|\n", current->key);
					// printf ("\n value : |%s|\n", current->value);
					if (count_dollar > 1)
						start = 1;
					ret = ft_strdup(current->value);
					break;
				}
				current = current->next;
			}
			// while (s[i] && s[i] == ' ')
			// 	i++;
			// s1 = ft_substr(s, 0, start - 1);
			// s2 = ft_substr(s, end, ft_strlen(s) - i);
			// s = ft_strjoin(s1, s2);
			if (!w && ret)
				w = ft_strdup(ret);
			// else if (!w && is_dollar)
			// 	w = ft_strdup(s1);
			else if (ret)
				w = ft_strjoin(w, ret);
			// else if (w && is_dollar)
			// 	w = ft_strdup(s1);
			i += ft_strlen(s1);
			if (ret)
				free(ret);
			if (s1)
				free(s1);
			// s1 = NULL;
			if (v)
				free(v);
			// i--;
			
			
			// s = ft_substr(s, i, ft_strlen(s) - i);
			// printf ("\n last s : |%s|\n", s);
			// free(s);
			// return (NULL);
		}
		else
		{
			s1 = str_export_split(s, i, 0);
			if (!w && s1)
				w = ft_strdup(s1);
			else if (s1)
				w = ft_strjoin(w, s1);
			i += ft_strlen(s1);
		}
		s1 = NULL;
		is_dollar = 0;
		current = g_env;
	}
	//free(s);
	if (w)
		return (w);
	else
		return (s);
}


/*char	*replace_dollar(char *s, char *v, int start, char *key, int end)
{
	char *s1;
	char *s2;
	char *s3;
	int l_key;
	int l_s;
	char *w;

	w = NULL;
	l_key = ft_strlen(key);
	l_s = ft_strlen(s);
	if (end)
		s3 = ft_substr(s, end, l_s - end);
	s1 = ft_substr(s, 0, start - 1);
	s2 = ft_substr(s, start + l_key, l_s - start - l_key);
	w = ft_strjoin(s1, v);
	w = ft_strjoin(w, s2);
	return (w);
}
int		equal_export(char *s, int i)
{
	i++;
	while (s[i] && s[i] != '$' && s[i] != '=' && s[i] != '/')
		i++;
	return (i);
}

char	*str_export_split(char *s, int end, int count_dollar)
{
	int i;
	int l;

	i = end - 1;
	l = 0;
	if (count_dollar == 1)
		return (ft_substr(s, 0, end));
	else
	{
		while (i > 0 && s[i] != '$')
		{
			l++;
			i--;
		}
		return (ft_substr(s, i, l + 1));
	}
}

char	*handling_dollar(char *s, t_sep *node)
{
	int		i;
	int		end;
	int		start;
	char    *s1;
	char    *s2;
	char	*ret;
	int		count_dollar;
	char	*v;
	char	*w;
	t_env	*current = g_env;
   
	i = 0;
	start = 0;
	s1 = NULL;
	s2 = NULL;
	end = 0;
	w = NULL;
	count_dollar = 0;
	while (s[i])
	{
		// start = 0;
		ret = NULL;
		if (ft_strlen(s) == 1 && s[0] == '$')
			return (ft_strdup("$"));
		if (s[i] == '$' || count_dollar > 0)
		{
			count_dollar++;
			if (node->is_builtin && (s[equal_export(s, i)] == '=' || s[equal_export(s, i)] == '/'))
				end = equal_export(s, i);
			if (s[i] == '$')
				start = ++i;
			while (s[i] && s[i] != ' ' && s[i] != '$')
				i++;
			v = ft_substr(s, start, i - start);
			if (end)
				v = ft_substr(s, start, end - start);
			// printf ("\n s : |%s|\n", s);
			// while (s[i] && s[i] != '$')
			// 	i++;
			s1 = str_export_split(s, i, count_dollar);
			while (current != NULL)
			{
				if (!ft_strcmp(current->key, v) || (!ft_strncmp(current->key, v, ft_strlen(current->key))
				&& v[ft_strlen(current->key)] == '\''))
				{
					// printf ("\n key : |%s|\n", current->key);
					// printf ("\n value : |%s|\n", current->value);
					if (count_dollar > 1)
						start = 1;
					ret = replace_dollar(s1, current->value, start, current->key, end);
					break;
				}
				current = current->next;
			}
			s1 = NULL;
			// while (s[i] && s[i] == ' ')
			// 	i++;
			// s1 = ft_substr(s, 0, start - 1);
			// s2 = ft_substr(s, end, ft_strlen(s) - i);
			// s = ft_strjoin(s1, s2);
			if (!w && ret)
				w = ft_strdup(ret);
			else if (ret)
				w = ft_strjoin(w, ret);
			if (ret)
				free(ret);
			if (s1)
				free(s1);
			free(v);
			// i--;
			current = g_env;
			// s = ft_substr(s, i, ft_strlen(s) - i);
			// printf ("\n last s : |%s|\n", s);
			// free(s);
			// return (NULL);
		}
		i++;
	}
	//free(s);
	if (w || count_dollar)
		return (w);
	else
		return (s);
	// echo $PATH$
	// unset $key2 (dont send args ex: unset $key where key="" or key)
	// echo $tttt test
}*/

char	*str_upper(char *s)
{
	int i;

	i = 0;
	while (s[i])
	{
		s[i] = ft_toupper(s[i]);
		i++;
	}
	return(s);
}

char	*str_lower(char *s)
{
	int i;

	i = 0;
	while (s[i])
	{
		s[i] = ft_tolower(s[i]);
		i++;
	}
	return(s);
}

void    add_to_args(int start, int end, char *s, int i, t_sep *node)
{
	int		l;
	char	type;
	char 	*str;
	int j;
	
	
	type = s[start];
	str = NULL;
	// while (s[start] && s[start] == ' ')
	// 	start++;
	if (i > 0)
	{
		if (node->is_path_in_arg)
			j = i;
		else
			j = i - 1;
			// if (j > 0)
		node->args = ft_realloc_2(node->args, j, (j + 1));
		node->args[j] = NULL;
	}
	while (s[start] && start < end)
	{
		if (s[start] != '\'' && s[start] != '\"')
		{
			l = start;
			while (s[l] && s[l] != '\'' && s[l] != '\"' && s[l] != ' ')
				l++;
			str = ft_substr(s, start, l - start);
			// start = l;
		}
		else
		{				
			l = search_second_quote(s, start + 1, s[start]);
			if (!l)
				error_msg("error multiligne");
			str = ft_substr(s, start + 1, l - start - 1);
		}
		// if (s[start] == '\"')
		// 	str = handling_bs_dq(str);
		// else if (s[start] != '\'')
		// 	str = handling_bs(str);
		if (s[start] != '\'')
			str = handling_dollar(str, node);
		if (i == 0)
		{
			handling_builtins(node, str, 0);
			// if (!node->builtin)
			// {
				// s = ft_strdup(handling_dollar(s, node));
				if (!node->is_builtin && node->path)
				{
					node->args = ft_realloc_2(node->args, i, (i + 1));
					node->args[i] = ft_strdup(node->path);
					node->args[i + 1] = NULL;
					node->is_path_in_arg = 1;
					// i++;
				}
			// }
			return ;
		}
		else
		{
			if (!ft_strcmp(node->lower_builtin, "unset") && !ft_strcmp(str, ""))
				free (str);
			if (!node->args[j] && str)
				node->args[j] = ft_strdup(str);
			else if (str)
				node->args[j] = ft_strjoin(node->args[j], str);		
			// printf ("\n\nARG[J] = |%s| \n\n", node->args[j]);
			if (str)
				free(str);
			str = NULL;
			// start = l;
			if (s[start] == '\'' || s[start] == '\"')
				start = l + 1;
			else
				start = l;
			// if (j > 0)
				
		}
		
	}
	node->args[j + 1] = NULL;
}

void	handling_builtins(t_sep *node, char *s, int start)
{
	int l;
	char *str;

	l = 0;
	str = NULL;
	if (s[start] != '\'' && s[start] != '\"')
	{
		l = start;
		while (s[l] && s[l] != '\'' && s[l] != '\"' && s[l] != ' ')
			l++;
		str = ft_substr(s, start, l - start);
		// start = l;
	}
	else
	{				
		l = search_second_quote(s, start + 1, s[start]);
		if (!l)
			error_msg("error multiligne");
		str = ft_substr(s, start + 1, l - start - 1);
	}
	node->builtin = ft_strdup(str);
	node->upper_builtin = ft_strdup(str_upper(str));
	node->lower_builtin = ft_strdup(str_lower(str));
	// if (!check_red(node, s) && !check_builtin(node) && !check_fill_path(node))
	check_fill_path(node);
	check_builtin(node);
}

void    get_args(char *s, int start, t_sep *node)
{
	int     i;
	int		end;
	int		t;
	int		j;

	i = 0;
	t = 0;
	j = 0;

	
	
	while (s[start])
	{
		// end = start;
		// if (s[start] == '\'' || s[start] == '\"')
		// {
		// 	if (s[start - 1] == '\\')
		// 		continue;
		// 	end = search_second_quote(s, start + 1, s[start]);
		// 	if (!end)
		// 	{
		// 		printf ("\n start = %d\n", start);
		// 		// error_msg("error multiligne");
		// 	}
			
		// }
		// while (s[end] && s[end] != ' ')
		// 	end++;
		t = start;
		
		while (s[start] && s[start] != ' ')
		{
			if (s[start] == '\'' || s[start] == '\"')
			{
				if (start - 1 > 0 && s[start - 1] == '\\')
					continue;
				end = search_second_quote(s, start + 1, s[start]);
				if (!end)
					error_msg("error multiligne");
				start = end + 1;
			}
			else
				start++;
		}
		// s = handling_dollar(s, node);
		
		if (t < start)
		{
			s = handling_dollar(s, node);
			add_to_args(t, start, s, i, node);
			i++;
		}
		// if (s[start] == '\'' || s[start] == '\"')
		// 	start = end + 1;
		// else
		// 	start = end;
		// start = end;
		while (s[start] && s[start] == ' ')
			start++;
	}
}

char	*red_redim_s(char *s, int start, int end)
{
	char	*s1;
	char	*s2;

	s1 = ft_substr(s, 0, start);
	s2 = ft_substr(s, end + 1, ft_strlen(s) - end + 1);
	s = ft_strjoin(s1, " ");
	s = ft_strjoin(s, s2);
	free(s1);
	free(s2);
	return(s);
}

char	*red_get_file(t_sep *node, char *s, int start, char type)
{
	int i;
	char *file;
	int end;

	i = 0;
	end = 0;
	file = NULL;
	while (s[start])
	{
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
				continue;
			end = search_second_quote(s, start + 1, s[start]);
			if (!end)
				error_msg("error multiligne");
			file = ft_substr(s, start + 1, end - start - 1);
			if (s[start] != '\'')
			{
				if (s[start] == '\"')
					file = handling_bs_dq(file);
				else
					file = handling_bs(file);
				file = handling_dollar(file, node);
			}
			node->red_args = red_redim_s(s, i, end);
			// printf("\n FILE : |%s| \n", file);
			return (file);
		}
		else
		{
			end = start;
			while (s[end] && s[end] != ' ' && s[end] != '<' && s[end] != '>')
				end++;
			file = ft_substr(s, start, end - start);
			node->red_args = red_redim_s(s, i, end - 1);
			if (s[start] != '\'')
			{
				if (s[start] == '\"')
					file = handling_bs_dq(file);
				else
					file = handling_bs(file);
				file = handling_dollar(file, node);
			}
			// printf("\n FILE : |%s| \n", file);
			return (file);
		}
		return (NULL);
	}
	return (NULL);
}

void    r_add_to_args(int start, int end, char *s, int i, t_sep *node)
{
	int		l;
	char	type;
	int j;
	
	type = s[start];
	l = end - start;
	// node->r_args[i] = NULL;

	if (i > 0)
	{
		if (node->is_path_in_arg)
			j = i;
		else
			j = i - 1;
			// if (j > 0)
		node->args = ft_realloc_2(node->args, j, (j + 1));
		node->args[j] = NULL;
	}
	if (i == 0)
	{
		handling_builtins(node, s, 0);
		// if (!node->builtin)
		// {
			// s = ft_strdup(handling_dollar(s, node));
			if (!node->is_builtin && node->path)
			{
				node->r_args = ft_realloc_2(node->r_args, i, (i + 1));
				node->r_args[i] = ft_strdup(node->path);
				node->r_args[i + 1] = NULL;
				node->is_path_in_arg = 1;
				// i++;
			}
		// }
		return ;
	}
	
		// node->r_args = ft_realloc_2(node->r_args, j, (j + 1));
		if (type != '\'' && type != '\"')
			node->r_args[j] = ft_substr(s, start, l);
		else
			node->r_args[j] = ft_substr(s, start + 1, l - 1);
		if (type != '\'')
		{
			if (type == '\"')
				node->r_args[j] = handling_bs_dq(node->r_args[j]);
			else
				node->r_args[j] = handling_bs(node->r_args[j]);
			node->r_args[j] = handling_dollar(node->r_args[j], node);
		}
	
	// node->r_args[i + 1] = NULL;
	printf ("\n\n\nred arg %d : |%s|\n", i, node->r_args[i]);
}

void	red_get_cmd_args(t_sep *node)
{
	int     i;
	int		end;
	int start;

	i = 0;
	start = 0;
	
	while (node->red_args[start])
	{
		if (node->red_args[start] == '\'' || node->red_args[start] == '\"')
		{
			if (node->red_args[start - 1] == '\\')
				continue;
			end = search_second_quote(node->red_args, start + 1, node->red_args[start]);
			if (!end)
				error_msg("error multiligne");
			// node->r_args = ft_realloc_2(node->r_args, i, (i + 1));
			// r_add_to_args(start, end, node->red_args, i, node);
			add_to_args(start, end, node->red_args, i, node);
			i++;
			start = end + 1;
		}
		else
		{
			end = start;
			while (node->red_args[end] && node->red_args[end] != ' ')
				end++;
			if (end > start)
			{
				// node->r_args = ft_realloc_2(node->r_args, i, (i + 1));
				// r_add_to_args(start, end, node->red_args, i, node);
				add_to_args(start, end, node->red_args, i, node);
				i++;
			}
			start = end;
		}
		while (node->red_args[start] && node->red_args[start] == ' ')
			start++;
	}
}
char	red_get_type(char *s, int start)
{
	int i;

	i = 0;
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
				if ((s[start + 2] == '<' && s[start + 3] != '<') ||
				(s[start + 2] == '<' && s[start + 3] == '<'))
					error_msg("syntax error near unexpected token `<'");
				return ('h');
			}
			return ('i');
		}
		start++;
	}
	return('\0');
}

void	red_get_type_file(t_sep *node, char *s, int start)
{
	int		i;
	// char	type;
	char	*file;
	int		end;
	int		j;

	i = 0;
	(void)node;
	file = NULL;
	end = 0;
	node->red_args = NULL;
	node->r_file = NULL;
	node->red_op = '\0';
	(void)start;
	j = 0;
	while (s[i])
	{
		while (s[i] && s[i] == ' ')
			i++;
		if (s[i] == '\'' || s[i] == '\"')
		{
			if (i != 0 && s[i - 1] == '\\')
				continue;
			end = search_second_quote(s, i + 1, s[i]);
			if (!end)
				error_msg("error multiligne");
			i = end;
		}
		if ((s[i] == '>' ) ||
		(s[i] == '<' ) )
		{
			// j = i;
			node->red_op = red_get_type(s, i);
			node->r_file = red_get_file(node, s, i, node->red_op);
			s = node->red_args;
			// printf("\n REDIM : |%s| \n", node->red_args);
			// printf("\n FILE : |%s| // TYPE : |%c| \n", file, type);
			// i = j;
		}
		if (node->r_file)
			{
				red_get_cmd_args(node);
				// printf("\n RED_ARGS : |%s| \n", node->red_args);
				// j = ft_strlen2(node->r_args);
				// i = 0;
				// while (j > 0)
				// {
				// 	printf("\narg %d : |%s|\n", i, node->r_args[i]);
				// 	i++;
				// 	j--; 
				// }
				// break;
				return ;
			}
		j = 0;
		i++;
	}
	// node->r_args = node->args;
}

void	init_t_sep(t_sep *node)
{
	node->path = NULL;
	node->t_sp = '\0';
	node->args = NULL;
	node->is_builtin = 0;
	node->builtin = NULL;
	node->upper_builtin = NULL;
	node->lower_builtin = NULL;
	node->is_red = 0;
	node->s_red = NULL;
	node->r_file = NULL;
	node->r_args = NULL;
	node->is_path_in_arg = 0;
	node->red_op = '\0';
}

int		check_red(t_sep *node, char *s)
{
	int start;
	int end;

	start = 0;
	end = 0;
	while (s[start])
	{
		if (s[start] == '\'' || s[start] == '\"')
		{
			if (s[start - 1] == '\\')
				continue;
			end = search_second_quote(s, start + 1, s[start]);
			if (!end)
				error_msg("error multiligne");
			start = end;
		}
		else
		{
			if ((start == 0 && (s[start] == '>' || s[start] == '<')) ||
			(start > 0 && (s[start] == '>' || s[start] == '<') && s[start - 1] != '\\'))
			{
				node->is_red = 1;
				red_get_type_file(node, s, start);
				// printf("\ntype red :  %c\n", s[start]);
				return (1);
			}
		}
		start++;
	}
	return (0);
}

char	*replace_from_env(char *str, t_sep *node)
{
	int i;
	char *tmp;
	int l;
	int fd;
	t_env *current = g_env;
	(void)node;
	i = 0;
	fd = 0;
	l = ft_strlen(str);
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1])
		{
			tmp = ft_substr(str, i + 1, l - i + 1);
			while (current != NULL)
			{
				if (!ft_strcmp(current->key, tmp))
				{
					free(str);
					free(tmp);
					return (ft_strdup(current->value)); 
				}
				current = current->next;
			}
		}
	}
	return (str);
}

void    get_builtin(char *s, t_sep *node)
{
	int     i;
	int		l;

	i = 0;
	// l = ft_strlen(s);
	// if (s[i] && s[i] == '$')
	// 	s = replace_from_env(s, node);
	l = ft_strlen(s);
	if (s[i] == '|')
	{
		i++;
		while (s[i] && s[i] == ' ')
			i++;
		s = ft_substr(s, i, l - i);
	}
	// i = l;
	
	// s = handling_dollar(s, node);
	// node->builtin = malloc(sizeof(char) * (i + 1));
	// node->upper_builtin = malloc(sizeof(char) * (i + 1));
	// node->lower_builtin = malloc(sizeof(char) * (i + 1));
	// i = 0;
	// while (s[i] && s[i] != ' ')
	// {
	// 	node->builtin[i] = s[i];
	// 	node->upper_builtin[i] = ft_toupper(s[i]);
	// 	node->lower_builtin[i] = ft_tolower(s[i]);
	// 	i++;
	// }
	// node->builtin[i] = '\0';
	// node->upper_builtin[i] = '\0';
	// node->lower_builtin[i] = '\0';
	// // if (!check_red(node, s) && !check_builtin(node) && !check_fill_path(node))
	// check_fill_path(node);
	// check_builtin(node);



	
		// error_msg("COMMAND NOT FOUND!!");
	// printf("\n\nred return : %d", node-> is_red);
	// if (node-> is_red != 1)
	// {
	i = 0;
	while (s[i] && s[i] == ' ')
		i++;
	if (!check_red(node, s))
		get_args(s, i, node);
	// printf("\ncmd : |%s|\n", node->builtin);
	// }
}
void	parsing_red(t_sep *node, char *s)
{
	int i;
	int end;

	i = 0;
	end = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '\"')
		{
			if (i != 0 && s[i - 1] == '\\')
				continue;
			end = search_second_quote(s, i + 1, s[i]);
			if (!end)
				error_msg("error multiligne");
			i = end;
		}
		else
		{
			if ((i == 0 && (s[i] == '>' || s[i] == '<')) ||
			(i > 0 && (s[i] == '>' || s[i] == '<') && s[i - 1] != '\\'))
			{
				node->is_red = 1;
				
			}
		}
		i++;
	}
}
void    fill_node(char *s, t_sep *node, int start, char *str)
{
	int i;
	int l;

	i = 0;
	l = ft_strlen(s);
	if (str[start + l] && (str[start + l] == '|' || str[start + l] == ';'))
		node->t_sp = str[start + l];
	// if (str[start] == '>' || str[start] == '<')
	// if (s[i] == '|')
	// 	s = ft_substr(s, i + 1, ft_strlen(s) - 1);
	// printf("\n s end : |%s|\n", s);
	// parsing_red(node, s);
	node->s_red = ft_strdup(s);
	
	
	// if (s[0] != '<' && s[0] != '>' && str[start + l] != '<' && str[start + l] != '>')
	// {
	// 	if (start > 0 && (str[start - 1] == '>' && str[start - 1] == '>'))
	// 		return ;
		get_builtin(s, node);
	// }


	
	// printf ("\n\n string red : |%s|", node->s_red);
	// printf ("\nlower : %s", node->cmd.lower_builtin);
	
	// if (!check_builtin(node))
	//     error_msg("there is no builtin or buitin not handled!!");
}


void	addlast_sep(t_sep **head, char *s, int start, char *str)
{
	int i;
	
	i = 0;
	(void)str;
	t_sep *newNode = malloc(sizeof(t_sep));
	t_sep *lastNode = *head;
	init_t_sep(newNode);
	fill_node(s, newNode, start, str);
	newNode->next = NULL;
	if (*head == NULL)
		 *head = newNode;
	else
	{
		lastNode = *head;
		while (lastNode->next != NULL)
		{
			lastNode = lastNode->next;
		}
		lastNode->next = newNode;
	}
}


void	free_mylist_sep(t_sep *head)
{
	t_sep *tmp;
	
	while (head != NULL)
	{
	   tmp = head;
	   head = head->next;
	   free(tmp);
	}
}

void	fill_list(char *str)
{
	int     i;
	int     start;
	char    *s;
	int		end;
	int     pipes_num;
	t_sep	*head;

	i = 0;
	end = 0;
	start = 0;
	head = NULL;
	pipes_num = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			if (i != 0 && str[i - 1] == '\\')
				continue;
			end = search_second_quote(str, i + 1, str[i]);
			if (!end)
				error_msg("error multiligne");
			i = end;
		}
		if ((str[i] == '|')
		// || (str[i] == ';')
		// || (str[i] == '>')
		// || (str[i] == '<')
		|| (!str[i + 1]))
		{
			pipes_num += (str[i] == '|');
			if (i && str[i - 1] == '\\')
				error_msg("error multiligne");
			while (str[start] && str[start] == ' ')
				start++;
			if (!str[i + 1])
				s = ft_substr(str, start, i - start + 1);
			else
		   		s = ft_substr(str, start, i - start);
			if (ft_strlen(s) > 0)
				addlast_sep(&head, s, start , str);
			if (s)
				free(s);
			start = i;
			// if ((str[i] == '>' && str[i + 1] == '>') || (str[i] == '<' && str[i + 1] == '<'))
			// 	i++;
		}
		i++;
	}
	print_mylist(head, pipes_num); 
	// run_cmdline(head, pipes_num);
	free_mylist_sep(head);
	
}

void    fill_args(char *str)
{
	int     i;
	
	i = 0;
	fill_list(str);
}


void    print_list_env()
{
	t_env *current = g_env;
	while (current != NULL)
	{
		printf("%s\n",current->key);
		current = current->next;
	}
}

t_env    *fill_env(char **env)
{
	t_env *head = NULL;
	t_env *tail = NULL;
	int i;
	i = 0;
	t_env *temp;
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
	return (head);
}

void    search_path()
{
	t_env *current = g_env;
	while (current != NULL)
	{
		if (ft_strcmp(current->key,"PWD") == 0)
		{
			// ft_putstr("\n");
			ft_putstr(current->value);
			// ft_putstr("\n");
			// current->value = pwd;
			break;
		}
		current = current->next;
	}
}

char    *my_getcwd()
{
	t_env *current = g_env;
	while(current != NULL)
	{
		if(ft_strcmp(current->key,"PWD") == 0)
		{
			return(ft_strjoin(current->value, "> "));
			break;
		}
		current = current->next;
	}
	return (NULL);
}

int     main(int argc, char **argv, char **env)
{
	int i;
	char *str;
	int ret;
	
	(void)argc;
	(void)argv;
	g_envp = env;
	g_env = fill_env(env);
	i = 0;
	ret = 0;
	while (1)
	{
		str = NULL;
		// str = readline(my_getcwd());
		str = readline("\x1B[32m$> \e[0m");
		if (ft_strlen(str) < 1)
		{
			free(str);
			continue;
		}
		add_history(str);
		if (!handling_errors_arg(str))
			fill_args(str);
		if (str)
			free(str);
		// system("leaks minishell");
	}
}