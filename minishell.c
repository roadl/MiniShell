#include "minishell.h"

int	exec_built_in(char *line, char **cmd, t_arg *arg)
{
	int	res;

	res = 0;
	printf("line: %s\n", line);
	if (ft_strncmp(line, "echo", 5) == 0)
		res = ft_echo(cmd, arg, 1);
	if (ft_strncmp(line, "cd", 3) == 0)
		res = ft_cd(cmd, arg);
	if (ft_strncmp(line, "pwd", 4) == 0)
		res = ft_pwd(cmd, arg);
	if (ft_strncmp(line, "export", 7) == 0)
		res = ft_export(cmd, arg);
	if (ft_strncmp(line, "unset", 6) == 0)
		res = ft_unset(cmd, arg);
	if (ft_strncmp(line, "env", 4) == 0)
		res = ft_env(cmd, arg);
	if (ft_strncmp(line, "exit", 5) == 0)
		res = ft_exit(cmd, arg);
	return (res);
}

// ctrl-C -> new lline
// ctrl-D -> exit
// ctrl-\ -> does nothing
// 들어올때 SHLVL 하나 증가시켜야 함
int	main(int argc, char **argv, char **envp)
{
	char		*line;
	char		**strs;
	t_arg		arg;

	if (argc != 1 || !init_arg(&arg, envp))
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
		exec_built_in(strs[0], strs, &arg);
		// set_signal_ignore?
		// 파싱
		add_history(line);
		free(line);
		// 실행
		// free, unlink
	}
}
