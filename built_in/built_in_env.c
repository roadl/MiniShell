/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yojin <yojin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:14:45 by yojin             #+#    #+#             */
/*   Updated: 2024/10/31 19:58:04 by yojin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 인자 없을때 아스키코드순으로 출력해야 함. 어떻게?
// 있어도 바뀌는게 아니라 추가만 된다.
int	ft_export(t_cmd *cmd, t_list **env_list, char ***envp)
{
	int		i;
	int		status;
	int		res;

	if (!cmd->argv[1])
		export_no_option(*env_list);
	res = EXIT_SUCCESS;
	i = 0;
	while (cmd->argv[++i])
	{
		if (ft_strncmp(cmd->argv[i], "_", 2) == 0)
			continue ;
		status = check_export_arg(cmd, *env_list, i);
		if (status == 2)
			return (2);
		if (status == 1)
			continue ;
		if (update_env(cmd->argv[i], env_list, envp) != 0)
			res = EXIT_FAILURE;
		i++;
	}
	return (res);
}

// option에 대해서 처리해야함
// exit_code 맞춰야 함
int	ft_unset(t_cmd *cmd, t_list **env_list, char ***envp)
{
	int	i;
	int	res;

	if (!cmd->argv[1])
		return (EXIT_SUCCESS);
	res = EXIT_SUCCESS;
	i = 1;
	while (cmd->argv[i])
	{
		if (i == 1 && cmd->argv[i][0] == '-')
		{
			print_error("unset", cmd->argv[i], \
				"invalid option", invalid_option);
			return (2);
		}
		if (unset_env(cmd->argv[i], env_list, envp) != 0)
			res = EXIT_FAILURE;
		i++;
	}
	return (res);
}

// 입력 인자 없어야 함
// = 이 있을때만 출력해야 함
int	ft_env(t_cmd *cmd, char ***envp)
{
	int		i;
	char	**_envp;

	_envp = *envp;
	if (cmd->argv[1] && cmd->argv[1][0] == '-' && cmd->argv[1][1] != '\0')
		return (print_error("env", cmd->argv[1], \
			"invalid option", invalid_option));
	if (cmd->argv[1])
	{
		ft_putstr_fd("env: ", STDERR_FILENO);
		ft_putstr_fd(cmd->argv[1], STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		return (127);
	}
	i = 0;
	while (_envp[i])
	{
		if (ft_strchr(_envp[i], '='))
			printf("%s\n", _envp[i]);
		i++;
	}
	return (EXIT_SUCCESS);
}

// 현재 디렉터리 주소 출력
int	ft_pwd(t_cmd *cmd)
{
	char	*cur;

	if (cmd->argv[1] && cmd->argv[1][0] == '-' && cmd->argv[1][1] != '\0')
		return (print_error("pwd", cmd->argv[1], \
			"invalid option", invalid_option));
	cur = getcwd(NULL, 0);
	if (!cur)
		handle_systemcall_error();
	printf("%s\n", cur);
	free(cur);
	return (0);
}
