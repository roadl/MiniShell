#include "minishell.h"

size_t	find_env_len(char *token)
{
	size_t	i = 0;

	// if (token[i] == '{')
	// {
	// 	i++;
	// 	while (token[i] && token[i] != '}')
	// 		i++;
	// }
	while (token[i] && (ft_isalnum(token[i]) || token[i] == '_'))
		i++;
	return i;
}

// 토큰화된 문자열을 받아서 따옴표 처리를 해주는 함수
// leak 가능성
char	*change_quotes(char *token, t_arg *arg)
{
	int 	i = 0;
	int 	j = 0;
	char	*new_token;
	char	*env_key;
	char	*env_value;

	new_token = (char *)malloc(sizeof(char) * ft_strlen(token) + 1);
	if (!new_token)
		return NULL;
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
					// 환경변수 처리
					i++;
					if (token[i] == '?')
						new_token = ft_strjoin_with_free(new_token, ft_itoa(arg->last_exit_code));
					else
					{
						//환경변수 키 찾고 ft_strjoin_with_free로 합쳐주기
						env_key = ft_substr(token, i, find_env_len(&token[i]));
						i += ft_strlen(env_key);
						env_value = get_env_value(find_env(env_key, arg->env_list));
						j += ft_strlen(env_value);
						new_token = ft_strjoin_with_free(new_token, env_value);
						free(env_key);
						continue ;
					}
				}
				new_token[j++] = token[i++];
			}
			i++;
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
		while (cmd->argv[i])
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
