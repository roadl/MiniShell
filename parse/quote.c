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

char	*replace_env_variable(char *token, int *i, t_arg *arg)
{
	char	*env_key;
	char	*env_value;
	char	*result;

	result = NULL;
	(*i)++;
	if (token[*i] == '?')
	{
		result = ft_itoa(g_exit_code);
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
	return (result);
}

char	*change_quotes(char *token, t_arg *arg, char *redi) // quote 마무리 안되는 경우 처리
{
	int		i = 0;
	int		j = 0;
	char	*new_token;
	char	*env_value;

	if (!token)
		return (NULL);
	new_token = (char *)malloc(sizeof(char) * ft_strlen(token) + 1);
	if (!new_token)
		handle_systemcall_error();
	ft_bzero(new_token, ft_strlen(token) + 1);
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
				if (token[i] == '$' && ft_strcmp(redi, "<<") != 0)
				{
					env_value = replace_env_variable(token, &i, arg);
					j += ft_strlen(env_value);
					new_token = ft_strjoin_with_free(new_token, env_value);
					continue ;
				}
				new_token[j++] = token[i++];
			}
			i++;
		}
		else if (token[i] == '$' && ft_strcmp(redi, "<<") != 0)
		{
			env_value = replace_env_variable(token, &i, arg);
			j += ft_strlen(env_value);
			new_token = ft_strjoin_with_free(new_token, env_value);
		}
		else
			new_token[j++] = token[i++];
	}
	free(token);
	new_token[j] = '\0';
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
		i = 0;
		while (cmd->cmd && cmd->argv[i])
		{
			cmd->argv[i] = change_quotes(cmd->argv[i], arg, NULL);
			i++;
		}
		redi = cmd->redi_list;
		while (redi)
		{
			((t_redi *)redi->content)->file
				= change_quotes(((t_redi *)redi->content)->file, arg, ((t_redi *)redi->content)->redi);
			redi = redi->next;
		}
		node = node->next;
	}
}
