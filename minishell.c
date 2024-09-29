#include "minishell.h"

#define READ 0
#define WRITE 1

// 들어올때 SHLVL 하나 증가시켜야 함
int	main(int argc, char **argv, char **envp)
{
	char			*line;
	t_arg			arg;
	int				cmd_count;

	(void)argv;
	if (argc != 1 || !init_arg(&arg, envp))
		exit(EXIT_FAILURE);
	while (1)
	{
		set_terminal_print_off();
		set_signal();
		line = readline("FastSHELL$ ");
		if (line == NULL)
		{
			do_sigterm();
			exit(EXIT_SUCCESS);
		}
		if (*line == '\0')
		{
			free(line);
			continue ;
		}
		add_history(line);
		arg.cmd_list = parsing(line, &cmd_count);
		if (!arg.cmd_list)
		{
			free(line);
			arg.last_exit_code = 258;
			continue ;
		}
		printf("hi\n");
		process_quotes(&arg);
		make_cmd(&arg);
		// print_cmd_list(arg.cmd_list);
		set_signal_fork();
		exec_cmds(&arg);
		free(line);
		ft_lstclear(&arg.cmd_list, free_cmd);
		// 실행
		// free, unlink
	}
}
