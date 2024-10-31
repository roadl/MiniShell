/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbel <jeongbel@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 06:11:04 by jeongbel          #+#    #+#             */
/*   Updated: 2024/10/31 19:53:44 by jeongbel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_single_quote(char *new, char *token, t_arg *arg)
{
	arg->index_old++;
	while (token[arg->index_old] && token[arg->index_old] != '\'')
		new[arg->index_new++] = token[arg->index_old++];
	if (token[arg->index_old] != '\'')
	{
		free(token);
		free(new);
		print_error("fastshell", NULL, "\'", error_syntax);
		return (1);
	}
	arg->index_old++;
	return (0);
}

int	process_double_quote(char **new, char *token, t_arg *arg, char *redi)
{
	arg->index_old++;
	while (token[arg->index_old] && token[arg->index_old] != '"')
	{
		if (token[arg->index_old] == '$' && ft_strcmp(redi, "<<") != 0)
		{
			if (process_dollar(new, token, arg))
				return (1);
			continue ;
		}
		*new = ft_stradd_with_free(*new, token[arg->index_old]);
		arg->index_new++;
		arg->index_old++;
	}
	if (token[arg->index_old] != '"')
	{
		free(token);
		free(*new);
		print_error("fastshell", NULL, "\"", error_syntax);
		return (1);
	}
	arg->index_old++;
	return (0);
}

int	process_dollar(char **new, char *token, t_arg *arg)
{
	char	*env_value;

	env_value = replace_env_variable(token, arg);
	if (!env_value)
		return (1);
	arg->index_new += ft_strlen(env_value);
	*new = ft_strjoin_with_free(*new, env_value);
	return (0);
}

void	init_quote_change(t_arg *arg, int *i, char *new, size_t len)
{
	*i = 0;
	ft_bzero(new, len);
	arg->index_old = 0;
	arg->index_new = 0;
}

char	*ft_stradd_with_free(char *s1, char s2)
{
	char	*join;
	size_t	i;

	join = (char *)malloc(sizeof(char) * (ft_strlen(s1) + 2));
	if (!join)
		handle_systemcall_error();
	i = 0;
	while (i < ft_strlen(s1))
	{
		join[i] = s1[i];
		i++;
	}
	join[i] = s2;
	join[i + 1] = '\0';
	free(s1);
	return (join);
}
