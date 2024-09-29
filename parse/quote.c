#include "minishell.h"

size_t	find_env_len(char *token)
{
	size_t	i = 0;

	while (token[i] && (ft_isalnum(token[i]) || token[i] == '_'))
		i++;
	return i;
}

char *replace_env_variable(char *token, int *i, t_arg *arg)
{
	char *env_key;
	char *env_value;
	char *result;

	result = NULL;
	(*i)++;
	if (token[*i] == '?')
	{
		result = ft_itoa(arg->last_exit_code);
		(*i)++;
	}
	else
	{
		env_key = ft_substr(token, *i, find_env_len(&token[*i]));
		*i += ft_strlen(env_key);
		env_value = get_env_value(find_env(env_key, arg->env_list));
		if (!env_value)
			result = ft_strdup("");
		else
			result = ft_strdup(env_value);
		free(env_key);
	}
	return result;
}

// 토큰화된 문자열을 받아서 따옴표 처리를 해주는 함수
// leak 가능성
char	*change_quotes(char *token, t_arg *arg)
{
	int 	i = 0;
	int 	j = 0;
	char	*new_token;

	if (!token)
		return NULL;
	new_token = (char *)malloc(sizeof(char) * ft_strlen(token) + 1);
	if (!new_token)
		handle_systemcall_error();
	while (token[i])
	{
		if (token[i] == '\'')
		{
			i++;
			while (token[i] && token[i] != '\'')
				new_token[j++] = token[i++];
			i++;
		}
		else if (token[i] == '"')
		{
			i++;
			while (token[i] && token[i] != '"')
			{
				if (token[i] == '$')
				{
					char *env_value = replace_env_variable(token, &i, arg);
					j += ft_strlen(env_value);
					new_token = ft_strjoin_with_free(new_token, env_value);
					continue;
				}
				new_token[j++] = token[i++];
			}
			i++;
		}
		else if (token[i] == '$')
		{
			char *env_value = replace_env_variable(token, &i, arg);
			j += ft_strlen(env_value);
			new_token = ft_strjoin_with_free(new_token, env_value);
		}
		else
			new_token[j++] = token[i++];
	}
	new_token[j] = '\0';
	return new_token;
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
		cmd->cmd = change_quotes(cmd->cmd, arg);
		i = 0;
		while (cmd->cmd && cmd->argv[i])
		{
			cmd->argv[i] = change_quotes(cmd->argv[i], arg);
			i++;
		}
		redi = cmd->redi_list;
		while (redi)
		{
			((t_redi *)redi->content)->file = change_quotes(((t_redi *)redi->content)->file, arg);
			redi = redi->next;
		}
		node = node->next;
	}
}
