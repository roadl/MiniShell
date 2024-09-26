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
		return (exec_built_in(node->content, arg, &arg->env_list, &arg->envp));
	while (node)
	{
		cmd = node->content;
		if (node == arg->cmd_list)
			cmd->read_fd = STDIN_FILENO;
		else
		{
			cmd->read_fd = fd[READ];
			close(fd[WRITE]);
		}
		if (node->next && pipe(fd) == -1)
			handle_systemcall_error();
		if (node->next)
			cmd->write_fd = fd[WRITE];
		else
			cmd->write_fd = STDOUT_FILENO;
		pid = run_child_process(arg, fd, node);
		node = node->next;
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		arg->last_exit_code = WEXITSTATUS(status);
	else
		arg->last_exit_code = 128 + WTERMSIG(status);
	wait(0);
	return (arg->last_exit_code);
}

void	handler(int signo)
{
	if (signo == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 1);
		rl_redisplay();
	}
}

void	setting_signal(void)
{
	signal(SIGINT, handler);
	signal(SIGQUIT, SIG_IGN);
}

// 들어올때 SHLVL 하나 증가시켜야 함
int	main(int argc, char **argv, char **envp)
{
	char			*line;
	t_arg			arg;
	int				cmd_count;
	struct termios	term;

	(void)argv;
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~(ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
	setting_signal();
	if (argc != 1 || !init_arg(&arg, envp))
		exit(EXIT_FAILURE);
	while (1)
	{
		// signal 설정
		line = readline("FastSHELL$ ");
		if (line == NULL)
		{
			printf("\033[1A");
            printf("\033[10C");
			printf(" exit\n");
			exit(-1);
		}
		if (*line == '\0')
		{
			free(line);
			continue ;
		}
		add_history(line);
		arg.cmd_list = parsing(line, &cmd_count);
		//print_cmd_list(arg.cmd_list);
		exec_cmds(&arg);
		free(line);
		ft_lstclear(&arg.cmd_list, free_cmd);
		// 실행
		// free, unlink
	}
}
