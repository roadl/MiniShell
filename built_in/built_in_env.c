#include "minishell.h"

// 인자 없을때 아스키코드순으로 출력해야 함. 어떻게?
// 있어도 바뀌는게 아니라 추가만 된다.
int	ft_export(char **cmd, t_arg *arg)
{
	char	*key;
	char	*value;
	int		i;

	if (!cmd[1])
		return (0);
	i = 1;
	while (cmd[i])
	{
		if (ft_strncmp(cmd[i], "_", 2) == 0)
		{
			i++;
			continue ;
		}
		if (i == 1 && cmd[i][0] == '-')
		{
			print_error("export", cmd[i], "invalid option", invalid_option);
			return (0);
		}
		update_env(cmd[i], arg);
		i++;
	}
}

// option에 대해서 처리해야함
// exit_code 맞춰야 함
int	ft_unset(char **cmd, t_arg *arg)
{
	int	i;

	if (!cmd[1])
		return (0);
	i = 1;
	while (cmd[i])
	{
		if (i == 1 && cmd[i][0] == '-')
		{
			printf("asdf\n");
			print_error("unset", cmd[i], "invalid option", invalid_option);
			return (0);
		}
		unset_env(cmd[i], arg);
		i++;
	}
}

// 입력 인자 없어야 함
// = 이 있을때만 출력해야 함
int	ft_env(char **cmd, t_arg *arg)
{
	int	i;

	if (cmd[1])
	{
		print_error("env", cmd[1], strerror(ENOENT), error_built_in);
		return (-1);
	}
	i = 0;
	while (arg->envp[i])
	{
		if (ft_strchr(arg->envp[i], '='))
			printf("%s\n", arg->envp[i]);
		i++;
	}
}