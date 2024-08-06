#include "built_in.h"
#include "../libft/libft.h"

int	ft_echo(char *cmd, int option, char **arg, char **envp)
{
	while (arg)
	{
		if ((*arg)[0] == '$')
		{
			// 환경 변수 찾아서 출력
		}
		printf("%s", *arg);
	}
	if (option)
		printf("\n");
	return (1);
}

int	ft_cd(char **cmd, char **envp)
{

}

int	ft_pwd(char **cmd, char **envp)
{

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
