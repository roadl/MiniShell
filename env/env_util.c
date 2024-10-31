/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yojin <yojin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:15:02 by yojin             #+#    #+#             */
/*   Updated: 2024/10/31 13:15:03 by yojin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		handle_systemcall_error();
	return (key);
}

// node에 있는 value dup해서 받아옴
// 에러시 함수 내부에서 exit
// 에러 아니면 value 반환, 없으면 NULL 반환
char	*get_env_value(t_list *node)
{
	char	*value;
	char	*temp;

	if (!node)
		return (NULL);
	temp = ft_strchr(node->content, '=');
	if (!temp)
		return (NULL);
	temp++;
	value = ft_strdup(temp);
	if (!value)
		handle_systemcall_error();
	return (value);
}

char	*get_shlvl(t_arg *arg)
{
	t_list	*node;
	char	*value;

	node = find_env("SHLVL", arg->env_list);
	if (!node)
		value = ft_strdup("0");
	else
		value = get_env_value(node);
	if (!value)
		handle_systemcall_error();
	return (value);
}
