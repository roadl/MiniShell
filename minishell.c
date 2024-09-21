#include "minishell.h"

#define READ 0
#define WRITE 1

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
		//print_envp(arg.envp);
		// 여기서 파싱해서 arg->cmd_list에 들어옴
		strs = ft_split(line, ' ');
		exec_cmds(&arg);
		// set_signal_ignore?
		// 파싱
		add_history(line);
		free(line);
		// 실행
		// free, unlink
	}
}
