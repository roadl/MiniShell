
#include "minishell.h"

int	exec_built_in(char *line, char **cmd, char **envp);

// ctrl-C -> new lline
// ctrl-D -> exit
// ctrl-\ -> does nothing
int	main(int argc, char **argv, char **envp)
{
	char		*line;
	const char	*test[] = { "-", "asdf wqer", NULL };
	char		**strs;

	if (argc != 1)
		exit(EXIT_FAILURE);
	while (1)
	{
		// signal 설정
		line = readline("fastshell$ ");
		if (line == NULL)
		{
			printf("line null\n");
			continue;
		}
		if (*line == '\0')
		{
			printf("line nothing\n");
			continue ;
		}
		strs = ft_split(line, ' ');
		exec_built_in(strs[0], strs, envp);
		// set_signal_ignore?
		// 파싱
		add_history(line);
		free(line);
		// 실행
		// free, unlink
	}
}

int	exec_built_in(char *line, char **cmd, char **envp)
{
	int	res;

	res = 0;
	if (ft_strncmp(line, "echo", 5) == 0)
		res = ft_echo(cmd, envp, 1);
	if (ft_strncmp(line, "cd", 3) == 0)
		res = ft_cd(cmd, envp);
	if (ft_strncmp(line, "pwd", 4) == 0)
		res = ft_pwd(cmd, envp);
	if (ft_strncmp(line, "export", 7) == 0)
		res = ft_export(cmd, envp);
	if (ft_strncmp(line, "unset", 6) == 0)
		res = ft_unset(cmd, envp);
	if (ft_strncmp(line, "env", 4) == 0)
		res = ft_env(cmd, envp);
	if (ft_strncmp(line, "exit", 5) == 0)
		res = ft_exit(cmd, envp);
}
