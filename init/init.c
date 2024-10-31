/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yojin <yojin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:15:18 by yojin             #+#    #+#             */
/*   Updated: 2024/10/31 13:15:18 by yojin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	shlvl_atoi(char *shlvl)
{
	int		i;
	int		result;

	i = 0;
	result = 0;
	if (!shlvl)
		handle_systemcall_error();
	if (shlvl[0] == '+')
		i = 1;
	if (shlvl[0] == '-')
		return (-1);
	while (shlvl[i])
	{
		if (!ft_isdigit(shlvl[i]))
			return (0);
		result = result * 10 + shlvl[i++] - '0';
		if (result >= 1000)
			break ;
	}
	return (result);
}

// SHLVL이 숫자가 아니면 1로 초기화함
static void	up_shlvl(t_arg *arg)
{
	char	*value;
	char	*temp;
	char	*str;
	int		level;

	value = get_shlvl(arg);
	temp = ft_strtrim(value, " ");
	level = shlvl_atoi(temp);
	free(temp);
	free(value);
	if (++level >= 1000)
	{
		printf("fastshell: warning: shell level too high (>= 1000), ");
		printf("resetting to 1\n");
		level = 1;
	}
	value = ft_itoa(level);
	if (!value)
		handle_systemcall_error();
	str = ft_strjoin("SHLVL=", value);
	if (!str)
		handle_systemcall_error();
	update_env(str, &arg->env_list, &arg->envp);
	free(value);
	free(str);
}

int	init_env_list(t_list **lst, char **envp)
{
	int		i;
	t_list	*new;

	i = 0;
	if (!envp)
		return (1);
	while (envp[i])
	{
		new = ft_lstnew(ft_strdup(envp[i]));
		if (!new || !new->content)
			handle_systemcall_error();
		ft_lstadd_back(lst, new);
		i++;
	}
	return (1);
}

char	**env_list_to_envp(t_list *lst, char **envp)
{
	int		size;
	int		i;
	t_list	*node;

	i = 0;
	if (envp)
		while (envp[i])
			free(envp[i++]);
	free(envp);
	size = ft_lstsize(lst);
	envp = (char **)malloc(sizeof(char *) * (size + 1));
	if (!envp)
		handle_systemcall_error();
	node = lst;
	i = 0;
	while (node)
	{
		envp[i] = ft_strdup(node->content);
		if (!envp[i])
			handle_systemcall_error();
		node = node->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

// last_exit_code 어떻게 할지 생각해야 함 ($?)
// 기존 STDIN, STDOUT 저장해놨다 써야할듯?
// built_in 하나만 들어오면 dup해야되는데 기존으로 바꿔줘야 함
int	init_arg(t_arg *arg, char **envp)
{
	arg->envp = NULL;
	arg->env_list = NULL;
	init_env_list(&arg->env_list, envp);
	arg->envp = env_list_to_envp(arg->env_list, arg->envp);
	arg->cmd_list = NULL;
	arg->last_exit_code = EXIT_SUCCESS;
	arg->origin_stdin = dup(STDIN_FILENO);
	arg->origin_stdout = dup(STDOUT_FILENO);
	up_shlvl(arg);
	update_env("OLDPWD", &arg->env_list, &arg->envp);
	update_pwd("PWD=", &arg->env_list, &arg->envp);
	return (1);
}
