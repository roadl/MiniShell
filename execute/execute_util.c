/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_util.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yojin <yojin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:15:08 by yojin             #+#    #+#             */
/*   Updated: 2024/10/31 15:18:14 by yojin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	while (1)
		if (wait(0) == -1)
			break ;
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

static void	check_access(t_cmd *cmd)
{
	if (access(cmd->cmd, F_OK) != 0)
	{
		ft_putstr_fd("fastshell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->argv[0], STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		exit(127);
	}
	if (access(cmd->cmd, X_OK) != 0 && access(cmd->cmd, F_OK) == 0)
	{
		ft_putstr_fd("fastshell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->argv[0], STDERR_FILENO);
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
		exit(126);
	}
}

void	check_cmd(t_cmd *cmd)
{
	struct stat	buf;

	lstat(cmd->cmd, &buf);
	if (S_ISDIR(buf.st_mode))
	{
		ft_putstr_fd("fastshell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->cmd, STDERR_FILENO);
		ft_putstr_fd(": is a directory\n", STDERR_FILENO);
		exit(126);
	}
	if (ft_strchr(cmd->cmd, '/') == 0)
	{
		ft_putstr_fd("fastshell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->argv[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		exit(127);
	}
	check_access(cmd);
}
