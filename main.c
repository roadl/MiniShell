#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "minishell.h"
#include "libft/libft.h"

int	ft_echo(char *cmd, int option, char **arg, char **envp)
{
	int		i;
	char	*env;

	i = 0;
	while (arg[i])
	{
		if (i != 0)
			printf(" ");
		if (arg[i][0] == '$' && arg[i][1] != '\0')
		{
			env = getenv(arg[i++] + 1);
			printf("%s", env);
			continue ;
		}
		printf("%s", arg[i++]);
	}
	if (option)
		printf("\n");
	return (1);
}

int	ft_cd(char *cmd, char **arg, char **envp)
{
	char	*target;

	if (ft_strncmp(arg[0], "-", 2) == 0)
		target = getenv("OLDPWD");
	if (ft_strncmp(arg[0], ".", 2) == 0)
		target = 
	if (ft_strncmp(arg[0], "..", 3) == 0)
	if (ft_strncmp(arg[0], "~", 2) == 0)
	if (ft_strncmp(arg[0], "/", 2) == 0)
}

int	ft_exit(char *cmd, int exit_code)
{
	printf("exit\n"); 
	exit_code %= 256;
	exit(exit_code);
}

int	main(int argc, char **argv, char **envp)
{
	char		*line;
	const char	*test[] = { "asdf", "$", NULL };

	if (argc != 1)
		exit(EXIT_FAILURE);
	while (1)
	{
		// signal 설정
		line = readline("fastshell$ ");
		if (line == NULL)
			printf("line null\n");
		if (*line == '\0')
			printf("line nothing\n");
		ft_echo("echo", 1, test, envp);
		// set_signal_ignore?
		// 파싱
		// add_history(line);
		free(line);
		// 실행
		// free, unlink
	}
}
