#include "minishell.h"

// node에 있는 key dup해서 돌려줌
// 에러시 함수 내부에서 exit
// 에러 아니면 Key 무조건 반환
char	*get_env_key(t_list *node)
{
	char	*key;
	char	*content;
	int		i;

	content = node->content;
	i = 0;
	while (content[i] && content[i] != '=')
		i++;
	key = ft_substr(content, 0, i);
	if (!key)
		print_error(NULL, NULL, NULL, error_systemcall);
	return (key);
}

// node에 있는 value dup해서 받아옴
// 에러시 함수 내부에서 exit
// 에러 아니면 value 반환, 없으면 NULL 반환
char	*get_env_value(t_list *node)
{
	char	*value;
	char	*temp;

	temp = ft_strchr(node->content, '=');
	if (!temp)
		return (NULL);
	value = ft_strdup(temp);
	if (!value)
		print_error(NULL, NULL, NULL, error_systemcall);
	return (value);
}
