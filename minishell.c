#include "minishell.h"

#define READ 0
#define WRITE 1

int	exec_cmds(t_arg *arg)
{
	t_list	*node;
	t_cmd	*cmd;
	int		fd[2];
	int		pid;
	int		status;

	node = arg->cmd_list;
	if (is_only_built_in(arg))
		return (exec_built_in(node->content, &arg->env_list, &arg->envp));
	while (node)
	{
		cmd = node->content;
		if (node != arg->cmd_list)
			cmd->read_fd = fd[READ];
		close(fd[WRITE]);
		if (node->next && pipe(fd) == -1)
			handle_systemcall_error();
		if (node->next)
			cmd->write_fd = fd[WRITE];
		pid = run_child_process(arg, fd, node);
		node = node->next;
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		arg->last_exit_code = WEXITSTATUS(status);
	else
		arg->last_exit_code = 128 + WTERMSIG(status);
	return (arg->last_exit_code);
}

// ctrl-C -> new lline
// ctrl-D -> exit
// ctrl-\ -> does nothing
// 들어올때 SHLVL 하나 증가시켜야 함
int	main(int argc, char **argv, char **envp)
{
	char		*line;
	t_arg		arg;
	int			cmd_count;		

	if (argc != 1 || !init_arg(&arg, envp))
		exit(EXIT_FAILURE);
	while (argv[0])
	{
		// signal 설정
		line = readline("fastshell$ ");
		if (line == NULL)
		{
			printf("line null\n");
			continue ;
		}
		if (*line == '\0')
		{
			printf("line nothing\n");
			continue ;
		}
		add_history(line);
		//printf("line: %s\n", line);
		//print_envp(arg.envp);
		// 여기서 파싱해서 arg->cmd_list에 들어옴
		arg.cmd_list = parsing(line, &cmd_count);
		//print_cmd_list(arg.cmd_list);
		exec_cmds(&arg);
		// set_signal_ignore?
		// 파싱
		add_history(line);
		free(line);
		ft_lstclear(&arg.cmd_list, free_cmd);
		// 실행
		// free, unlink
	}
	argv = NULL;
}
