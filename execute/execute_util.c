#include "minishell.h"

#define READ 0
#define WRITE 1

int	wait_childs(t_arg *arg, int f_pid, int l_pid)
{
	int	statloc;

	waitpid(f_pid, &statloc, 0);
	if ((statloc & 255) != 0)
	{
		if ((statloc & 127) == 2)
			ft_putstr_fd("\n", STDERR_FILENO);
		else if ((statloc & 127) == 3)
			ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
	}
	if (l_pid != 0)
		waitpid(l_pid, &statloc, 0);
	if (WIFEXITED(statloc))
		arg->last_exit_code = WEXITSTATUS(statloc);
	else
		arg->last_exit_code = 128 + WTERMSIG(statloc);
	wait(0);
	return (arg->last_exit_code);
}

void	dup_fd(t_cmd *cmd)
{
	if ((dup2(cmd->read_fd, STDIN_FILENO) == -1) || \
		(dup2(cmd->write_fd, STDOUT_FILENO) == -1))
		handle_systemcall_error();
	if (cmd->read_fd != STDIN_FILENO)
		close(cmd->read_fd);
	if (cmd->write_fd != STDOUT_FILENO)
		close(cmd->write_fd);
}

void	set_fd(t_arg *arg, t_cmd *cmd, t_list *node, int fd[2])
{
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
}
