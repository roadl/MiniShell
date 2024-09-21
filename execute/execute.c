#include "minishell.h"

# define READ 0
# define WRITE 1

void	execute_command(t_arg *arg, t_cmd *cmd)
{
	if (dup2(cmd->read_fd, STDIN_FILENO) == -1 || \
		dup2(cmd->write_fd, STDOUT_FILENO) == -1)
		handle_systemcall_error();
	close(cmd->read_fd);
	close(cmd->write_fd);
	if (access(cmd->cmd, X_OK) != 0)
	{
		ft_putstr_fd("pipex: command not found: ", STDERR_FILENO);
		ft_putstr_fd(cmd->argv[0], STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	if (execve(cmd->cmd, cmd->argv, arg->envp))
		handle_systemcall_error();
	return ;
}

int	exec_built_in(t_arg *arg, t_cmd *cmd)
{
	int		res;
	char	*command;
	char	*argv;

	res = 0;
	printf("line: %s\n", cmd->cmd);
	if (ft_strncmp(cmd->cmd, "echo", 5) == 0)
		res = ft_echo(cmd, arg, 1);
	if (ft_strncmp(cmd->cmd, "cd", 3) == 0)
		res = ft_cd(cmd, arg);
	if (ft_strncmp(cmd->cmd, "pwd", 4) == 0)
		res = ft_pwd(cmd, arg);
	if (ft_strncmp(cmd->cmd, "export", 7) == 0)
		res = ft_export(cmd, arg);
	if (ft_strncmp(cmd->cmd, "unset", 6) == 0)
		res = ft_unset(cmd, arg);
	if (ft_strncmp(cmd->cmd, "env", 4) == 0)
		res = ft_env(cmd, arg);
	if (ft_strncmp(cmd->cmd, "exit", 5) == 0)
		res = ft_exit(cmd, arg);
	return (res);
}

int	run_child_process(t_arg *arg, int *fd, t_list *node)
{
	t_cmd	*cmd;
	int		pid;

	cmd = node->content;
	pid = fork();
	if (pid == -1)
		handle_systemcall_error();
	if (pid == 0)
	{
		if (node->next)
			close(fd[READ]);
		else
			cmd->write_fd = open(arg->outfile, \
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (node == arg->commands->next)
			cmd->read_fd = open(arg->infile, O_RDONLY);
		if (cmd->read_fd < 0 || info->write_fd < 0)
			error_exit();
		if (is_built_in(cmd->cmd))
			exec_built_in(cmd, arg)
		execute_command(arg, cmd);
	}
	return (pid);
}

int	exec_cmds(t_arg *arg)
{
	t_list	*node;
	t_cmd	*cmd;
	int		fd[2];
	int		pid;
	int		status;

	node = arg->cmd_list;
	if (is_only_built_in(arg))
		return (exec_built_in(node->content, arg));
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
		pid = run_child_process(arg, fd, cmd);
		node = node->next;
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		arg->last_exit_code = WEXITSTATUS(status);
	else
		arg->last_exit_code = 128 + WTERMSIG(status);
}
