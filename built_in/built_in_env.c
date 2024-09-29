#include "minishell.h"

static int	print_export(t_list *env_list)
{
	t_list	*node;
	char	*key;
	char	*value;

	node = env_list;
	while (node)
	{
		if (ft_strncmp(node->content, "", 1) == 0)
		{
			node = node->next;
			continue ;
		}
		key = get_env_key(node);
		value = get_env_value(node);
		printf("declare -x %s", key);
		if (value)
			printf("=\"%s\"", value);
		printf("\n");
		free(key);
		free(value);
		node = node->next;
	}
	return (0);
}

// 인자 없을때 아스키코드순으로 출력해야 함. 어떻게?
// 있어도 바뀌는게 아니라 추가만 된다.
int	ft_export(t_cmd *cmd, t_list **env_list, char ***envp)
{
	int		i;

	if (!cmd->argv[1])
		print_export(*env_list);
	i = 1;
	while (cmd->argv[i])
	{
		if (ft_strncmp(cmd->argv[i], "_", 2) == 0)
		{
			i++;
			continue ;
		}
		if (i == 1 && cmd->argv[i][0] == '-')
		{
			print_error("export", cmd->argv[i], \
				"invalid option", invalid_option);
			return (0);
		}
		update_env(cmd->argv[i], env_list, envp);
		i++;
	}
	return (EXIT_SUCCESS);
}

// option에 대해서 처리해야함
// exit_code 맞춰야 함
int	ft_unset(t_cmd *cmd, t_list **env_list, char ***envp)
{
	int	i;

	if (!cmd->argv[1])
		return (EXIT_FAILURE);
	i = 1;
	while (cmd->argv[i])
	{
		if (i == 1 && cmd->argv[i][0] == '-')
		{
			print_error("unset", cmd->argv[i], \
				"invalid option", invalid_option);
			return (0);
		}
		unset_env(cmd->argv[i], env_list, envp);
		i++;
	}
	return (EXIT_SUCCESS);
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
		return (print_error("env", cmd->argv[1], \
			strerror(ENOENT), error_built_in));
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
