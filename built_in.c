#include "minishell.h"

// built_in 함수 전체 함수 하나로 처리해야할듯
// 리턴값 보고 에러시 exit? 어떻게 해야 하지

// option -n 들어올 수 있음
// $?에 대한 처리 어떻게 할지 생각해야 함
int	ft_echo(char **arg, char **envp, int option)
{
	while (*arg)
	{
		printf("%s", *arg);
		arg++;
	}
	if (option)
		printf("\n");
	return (1);
}

// 상대, 절대 경로
// 인자가 1개면 해당 경로 변경
// 2개면 처음 나온 첫번째 문자열을 두번째 문자열로 대치
// 3개부턴 too many arguments 에러
int	ft_cd(char **cmd, char **envp)
{
	char	*cur;

	cur = getcwd(NULL, 0);
	printf("Current dir: %s\n", cur);
	if (chdir(cmd[0]) == -1)
		perror(cmd[0]);
	cur = getcwd(NULL, 0);
	printf("Changed dir: %s\n", cur);
	free(cur);
}

// 현재 디렉터리 주소 출력
int	ft_pwd(char **cmd, char **envp)
{
	char	*cur;

	cur = getcwd(NULL, 0);
	if (!cur)
	{
		perror(0);
		return (-1);
	}
	printf("%s\n", cur);
	free(cur);
	return (0);
}

int	ft_export(char **cmd, char **envp)
{

}

int	ft_unset(char **cmd, char **envp)
{

}

int	ft_env(char **cmd, char **envp)
{

}

int	ft_exit(char **cmd, char **envp)
{

}
