/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbel <jeongbel@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 05:43:07 by jeongbel          #+#    #+#             */
/*   Updated: 2024/10/31 19:27:31 by jeongbel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit_code;

size_t	find_env_len(char *token)
{
	size_t	i;

	i = 0;
	while (token[i] && (ft_isalnum(token[i]) || token[i] == '_'))
		i++;
	return (i);
}

char	*replace_env_variable(char *token, t_arg *arg)
{
	char	*env_key;
	char	*env_value;
	char	*result;

	result = NULL;
	arg->index_old++;
	if (token[arg->index_old] == '?')
	{
		result = ft_itoa(g_exit_code);
		arg->index_old++;
	}
	else
	{
		env_key = ft_substr(token, arg->index_old,
				find_env_len(&token[arg->index_old]));
		arg->index_old += ft_strlen(env_key);
		env_value = get_env_value(find_env(env_key, arg->env_list));
		if (!env_value)
			result = ft_strdup("");
		else
			result = ft_strdup(env_value);
		free(env_key);
		free(env_value);
	}
	return (result);
}

char	*change_quotes(char *token, t_arg *arg, char *redi)
{
	char	*new_token;
	int		i;

	if (!token)
		return (NULL);
	new_token = (char *)malloc(sizeof(char) * ft_strlen(token) + 1);
	if (!new_token)
		handle_systemcall_error();
	init_quote_change(arg, &i, new_token, ft_strlen(token) + 1);
	while (token[arg->index_old])
	{
		if (token[arg->index_old] == '\'')
			i = process_single_quote(new_token, token, arg);
		else if (token[arg->index_old] == '"')
			i = process_double_quote(&new_token, token, arg, redi);
		else if (token[arg->index_old] == '$' && ft_strcmp(redi, "<<") != 0)
			i = process_dollar(&new_token, token, arg);
		else
			new_token[arg->index_new++] = token[arg->index_old++];
		if (i)
			return (NULL);
	}
	free(token);
	new_token[arg->index_new] = '\0';
	return (new_token);
}

void	process_quotes(t_arg *arg)
{
	t_list	*node;
	t_cmd	*cmd;
	t_list	*redi;
	int		i;

	node = arg->cmd_list;
	while (node)
	{
		cmd = node->content;
		cmd->cmd = change_quotes(cmd->cmd, arg, NULL);
		i = -1;
		while (cmd->cmd && cmd->argv[++i])
			cmd->argv[i] = change_quotes(cmd->argv[i], arg, NULL);
		redi = cmd->redi_list;
		while (redi)
		{
			((t_redi *)redi->content)->file
				= change_quotes(((t_redi *)redi->content)->file, arg,
					((t_redi *)redi->content)->redi);
			redi = redi->next;
		}
		node = node->next;
	}
}
