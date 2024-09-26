#include "minishell.h"

#define READ 0
#define WRITE 1

void	execute_command(t_arg *arg, t_cmd *cmd)
{
	handle_redi(cmd);
	if ((dup2(cmd->read_fd, STDIN_FILENO) == -1) || \
		(dup2(cmd->write_fd, STDOUT_FILENO) == -1))
		handle_systemcall_error();
	if (cmd->read_fd != STDIN_FILENO)
		close(cmd->read_fd);
	if (cmd->write_fd != STDOUT_FILENO)
		close(cmd->write_fd);
	if (ft_strchr(cmd->cmd, '/') == 0)
	{
		ft_putstr_fd("fastshell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->argv[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		exit(127);
	}
	if (access(cmd->cmd, X_OK) != 0 && access(cmd->cmd, F_OK) == 0)
	{
		ft_putstr_fd("fastshell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->argv[0], STDERR_FILENO);
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
		exit(126);
	}
	if (execve(cmd->cmd, cmd->argv, arg->envp))
		handle_systemcall_error();
	return ;
}

int	exec_built_in(t_cmd *cmd, t_arg *arg, t_list **env_list, char ***envp)
{
	int		res;

	handle_redi(cmd);
	if (dup2(cmd->read_fd, STDIN_FILENO) == -1 || \
		dup2(cmd->write_fd, STDOUT_FILENO) == -1)
		handle_systemcall_error();
	res = 0;
	if (ft_strncmp(cmd->cmd, "echo", 5) == 0)
		res = ft_echo(cmd, 1);
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
	if (dup2(STDIN_FILENO, arg->origin_stdin) == -1)
		handle_systemcall_error();
	if (dup2(STDOUT_FILENO, arg->origin_stdout) == -1)
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
	//printf("run_child: %s, %p\n", cmd->cmd, fd);
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
