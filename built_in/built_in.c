#include "minishell.h"

// built_in 함수 전체 함수 하나로 처리해야할듯
// 리턴값 보고 에러시 exit? 어떻게 해야 하지

// option -n 들어올 수 있음
// $?에 대한 처리 어떻게 할지 생각해야 함
int	ft_echo(char **cmd, t_arg *arg, int option)
{
	while (*cmd)
	{
		printf("%s", *cmd);
		cmd++;
	}
	if (option)
		printf("\n");
	return (0);
}

// 상대, 절대 경로
// $HOME은 파싱에서 바꿔서 전달해줄거임
// OLDPWD 설정해야 함
// - or ~면 getenv로 가져와서 path로 chdir
// 아니면 그냥 들어온거로 chdir
// 바꾸면 이전거 OLDPWD, 다음거 PWD로 보내야 함
int	ft_cd(char **cmd, t_arg *arg)
{
	char	*cur;
	char	*path;

	path = 0;
	if (!cmd[1])
		return (0);
	if (ft_strncmp(cmd[1], "-", 2) == 0)
		path = getenv("OLDPWD");
	else if (ft_strncmp(cmd[1], "~", 2) == 0)
	{
		path = getenv("HOME");
		printf("%s\n", path);
	}
	else
		return (chdir(cmd[1]));
	if (!path || chdir(path) == -1)
	{
		free(path);
		return (-1);
	}
	return (0);
}

// 현재 디렉터리 주소 출력
int	ft_pwd(char **cmd, t_arg *arg)
{
	char	*cur;

	cur = getcwd(NULL, 0);
	if (!cur)
	{
		perror("pwd: ");
		return (-1);
	}
	printf("%s\n", cur);
	free(cur);
	return (0);
}

// 인자 안 들어오면 처리
// envp에서 가져와서
int	ft_export(char **cmd, t_arg *arg)
{
	char	*key;
	char	*value;
	int		i;
	
	if (!cmd[1])
		return (0);
	i = 0;
	while (arg->envp[i])
	{
		if (ft_strncmp(cmd[1], arg->envp[i], ft_strlen(cmd[1])) == 0)
			break;
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

// 인자 없음 0
// 인자 있음 % 256
// 숫자 아니면 에러메세지
// 인자 2개 이상이면 에러메세지
// single command일때 exit 출력 후 종료
// 음수일땐 256을 뺀 값
// 숫자가 long long 넘어가면 에러메세지
int	ft_exit(char **cmd, t_arg *arg)
{
	int	exit_code;

	if (!cmd[1])
		exit(EXIT_SUCCESS);
	exit_code = ft_atoi(cmd[1]) % 256;
	if (exit_code < 0)
		exit_code = exit_code + 256;
	exit(exit_code);
}
