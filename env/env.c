#include "minishell.h"

static int	is_valid_key(const char *key)
{
	int	i;

	if (!key)
		return (0);
	if (!ft_isalpha(key[0]) && key[0] != '_')
		return (0);
	i = 1;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

/// @brief find env, key값으로 env 돌면서 찾아서 그 문자열 주소 리턴, 없으면 null
/// @return success string address, fail 0
t_list	*find_env(const char *key, t_arg *arg)
{
	t_list	*node;
	char	*content;

	node = arg->env_list;
	while (node)
	{
		content = node->content;
		printf("%s\n", content);
		if (ft_strncmp(key, content, ft_strlen(key)) == 0 \
			&& content[ft_strlen(key)] == '=')
			return (node);
		node = node->next;
	}
	return (0);
}

/// @brief update env, 있으면 덮어쓰기, 없으면 만들어서 넣기
/// @return success 0, fail -1
// key value가 아니라 문자열로 들어온거 = 기준으로 잘라서 앞에꺼 key로 써야 할듯
int	update_env(const char *key, const char *value, t_arg *arg)
{
	t_list	*node;

	if (!is_valid_key(key))
	{
		print_error("export", "key", "not a valid identifier", invalid_identifier);
		return (-1);
	}
	node = find_env(key, arg->envp);
	if (!node)
	{
		// 새로 노드 만들어서 붙여주기 or 비어있는 노드 찾아서 거따 넣어주기
	}
	// 원래 있던거 할당 해제하고 새로 만들어서 넣어주기
	// free(*str);
	// *str = ft_strdup("");
	// if (!str)
	// 	return (-1);
}

int	unset_env(const char *key, t_arg *arg)
{
	t_list	*node;

	if (key[0] == '-')
	{
		print_error("unset", key, "invalid option", invalid_option);
		return (-1);
	}
	if (!is_valid_key(key))
	{
		print_error("unset", key, "not a valid identifier", invalid_identifier);
		return (-1);
	}
	node = find_env(key, arg->envp);
	printf("find!: %s\n", node->content);
	if (!node)
		return (0);
	free(node->content);
	printf("free\n");
	node->content = ft_strdup("");
	if (!node->content)
		print_error(NULL, NULL, NULL, error_systemcall);
	return (env_list_to_envp(arg));
}
