#include "minishell.h"

static int	is_valid_key(const char *key, t_key_type type)
{
	int	i;

	if (!key)
		return (0);
	if (!ft_isalpha(key[0]) && key[0] != '_')
		return (0);
	i = 1;
	while (key[i])
	{
		if (key[i] == '=' && type == EXPORT)
			return (1);
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

/// @brief find env, key값으로 env 돌면서 찾아서 그 문자열 주소 리턴, 없으면 null
/// @return success string address, fail 0
t_list	*find_env(const char *key, t_list *env_list)
{
	t_list	*node;
	char	*content;
	int		len;

	node = env_list;
	len = ft_strlen(key);
	while (node)
	{
		content = node->content;
		if (ft_strncmp(key, content, len) == 0 && \
				(content[len] == '\0' || content[len] == '='))
			return (node);
		node = node->next;
	}
	return (0);
}

/// @brief update env, 있으면 덮어쓰기, 없으면 만들어서 넣기
/// @return success 0, fail -1
// key value가 아니라 문자열로 들어온거 = 기준으로 잘라서 앞에꺼 key로 써야 할듯
int	update_env(char *str, t_list **env_list, char ***envp)
{
	t_list	*node;
	char	*key;

	if (!is_valid_key(str, EXPORT))
		return (print_error("export", str, "not a valid identifier", \
			invalid_identifier));
	key = ft_substr(str, 0, ft_strchr(str, '=') - str);
	if (!key)
		handle_systemcall_error();
	node = find_env(key, *env_list);
	if (!node)
	{
		node = ft_lstnew(NULL);
		ft_lstadd_back(env_list, node);
	}
	else
		free(node->content);
	node->content = ft_strdup(str);
	if (!node || !node->content)
		handle_systemcall_error();
	free(key);
	*envp = env_list_to_envp(*env_list, *envp);
	return (0);
}

int	unset_env(char *key, t_list **env_list, char ***envp)
{
	t_list	*node;

	if (!is_valid_key(key, UNSET))
	{
		print_error("unset", key, "not a valid identifier", invalid_identifier);
		return (-1);
	}
	node = find_env(key, *env_list);
	if (!node)
		return (0);
	free(node->content);
	node->content = ft_strdup("");
	if (!node->content)
		handle_systemcall_error();
	*envp = env_list_to_envp(*env_list, *envp);
	return (0);
}
