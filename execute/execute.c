#include "minishell.h"

#define READ 0
#define WRITE 1

void	execute_command(t_arg *arg, t_cmd *cmd)
{
	if (handle_redi(cmd) == -1)
		exit(EXIT_FAILURE);
	set_signal_origin();
	set_terminal_print_on();
	if (!cmd->cmd)
		exit(EXIT_SUCCESS);
	dup_fd(cmd);
	check_cmd(cmd);
	if (execve(cmd->cmd, cmd->argv, arg->envp))
		handle_systemcall_error();
	return ;
}

int	exec_built_in(t_cmd *cmd, t_arg *arg, t_list **env_list, char ***envp)
{
	int		res;

	if (handle_redi(cmd) == -1)
		return (EXIT_FAILURE);
	dup_fd(cmd);
	res = 0;
	if (ft_strncmp(cmd->cmd, "echo", 5) == 0)
		res = ft_echo(cmd);
	if (ft_strncmp(cmd->cmd, "cd", 3) == 0)
		res = ft_cd(cmd, env_list, envp);
	if (ft_strncmp(cmd->cmd, "pwd", 4) == 0)
		res = ft_pwd(cmd);
	if (ft_strncmp(cmd->cmd, "export", 7) == 0)
		res = ft_export(cmd, env_list, envp);
	if (ft_strncmp(cmd->cmd, "unset", 6) == 0)
		res = ft_unset(cmd, env_list, envp);
	if (ft_strncmp(cmd->cmd, "env", 4) == 0)
		res = ft_env(cmd, envp);
	if (ft_strncmp(cmd->cmd, "exit", 5) == 0)
		res = ft_exit(cmd);
	if (dup2(arg->origin_stdin, STDIN_FILENO) == -1)
		handle_systemcall_error();
	if (dup2(arg->origin_stdout, STDOUT_FILENO) == -1)
		handle_systemcall_error();
	return (res);
}

int	exec_built_in_child(t_cmd *cmd, t_arg *arg, char **envp)
{
	t_list	*env_list;
	char	**new_envp;
	int		res;

	env_list = NULL;
	new_envp = NULL;
	init_env_list(&env_list, envp);
	new_envp = env_list_to_envp(env_list, new_envp);
	res = exec_built_in(cmd, arg, &env_list, &new_envp);
	ft_lstclear(&env_list, free);
	free_strs(new_envp);
	exit(res);
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
		if (cmd->read_fd < 0 || cmd->write_fd < 0)
			handle_systemcall_error();
		if (is_built_in(cmd->cmd))
			exec_built_in_child(cmd, arg, arg->envp);
		else
			execute_command(arg, cmd);
	}
	return (pid);
}

int	exec_cmds(t_arg *arg)
{
	t_list	*node;
	int		fd[2];
	int		old_read_fd;
	int		l_pid;
	int		f_pid;

	l_pid = 0;
	node = arg->cmd_list;
	if (is_only_built_in(arg))
		return (exec_built_in(node->content, arg, &arg->env_list, &arg->envp));
	while (node)
	{
		old_read_fd = fd[READ];
		set_fd(arg, node->content, node, fd);
		if (node == arg->cmd_list)
			f_pid = run_child_process(arg, fd, node);
		else
			l_pid = run_child_process(arg, fd, node);
		if (node != arg->cmd_list)
			close(old_read_fd);
		node = node->next;
	}
	return (wait_childs(arg, f_pid, l_pid));
}
