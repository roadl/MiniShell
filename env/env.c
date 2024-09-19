#include "minishell.h"

typedef enum e_key_type {
	EXPORT,
	UNSET
}	t_key_type;

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

static char	*ft_strjoin_c(char const *s1, char const *s2, char c)
{
	char	*str;
	size_t	s_len;
	size_t	i;

	s_len = ft_strlen(s1);
	str = malloc((s_len + ft_strlen(s2) + 2) * sizeof(char));
	if (!str)
		return (0);
	i = 0;
	while (*(s1 + i))
	{
		*(str + i) = *(s1 + i);
		i++;
	}
	str[i] = 'c';
	i = 0;
	while (*(s2 + i))
	{
		*(str + s_len + i + 1) = *(s2 + i);
		i++;
	}
	*(str + s_len + i + 1) = '\0';
	return (str);
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
int	update_env(const char *str, t_arg *arg)
{
	t_list	*node;
	char	*key;
	char	*content;

	if (!is_valid_key(str, EXPORT))
	{
		print_error("export", str, "not a valid identifier", \
			invalid_identifier);
		return (-1);
	}
	key = ft_substr(str, 0, ft_strchr(str, '=') - str);
	if (!key)
		print_error(NULL, NULL, NULL, error_systemcall);
	node = find_env(key, arg);
	if (!node)
	{
		node = ft_lstnew(ft_strdup(str));
		ft_lstadd_back(&arg->env_list, node);
	}
	node->content = ft_strdup(str);
	if (!node || !node->content)
		print_error(NULL, NULL, NULL, error_systemcall);
	free(key);
	return (env_list_to_envp(arg));
}

int	unset_env(const char *key, t_arg *arg)
{
	t_list	*node;

	if (!is_valid_key(key, UNSET))
	{
		print_error("unset", key, "not a valid identifier", invalid_identifier);
		return (-1);
	}
	node = find_env(key, arg);
	if (!node)
		return (0);
	free(node->content);
	node->content = ft_strdup("");
	if (!node->content)
		print_error(NULL, NULL, NULL, error_systemcall);
	return (env_list_to_envp(arg));
}
