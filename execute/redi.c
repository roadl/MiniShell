/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redi.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yojin <yojin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:15:16 by yojin             #+#    #+#             */
/*   Updated: 2024/10/31 13:15:16 by yojin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define READ 0
#define WRITE 1

static int	open_file(char *file, int option, int type, t_cmd *cmd)
{
	if (type == READ && cmd->read_fd != STDIN_FILENO)
		close(cmd->read_fd);
	if (type == WRITE && cmd->write_fd != STDOUT_FILENO)
		close(cmd->write_fd);
	if (type == READ)
	{
		if (access(file, F_OK) != 0)
		{
			print_error(NULL, file, "No such file or directory", error_file);
			return (-1);
		}
		if (access(file, R_OK) != 0)
		{
			print_error(NULL, file, "Permission denied", error_access);
			return (-1);
		}
		cmd->read_fd = open(file, option);
	}
	else
		cmd->write_fd = open(file, option, 0644);
	if (cmd->read_fd < 0 || cmd->write_fd < 0)
		handle_systemcall_error();
	return (0);
}

static void	do_here_doc(t_cmd *cmd, t_redi *redi)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putstr_fd("\033[1A", STDIN_FILENO);
			ft_putstr_fd("\033[2C", STDIN_FILENO);
			break ;
		}
		if (ft_strlen(line) == 0 && ft_strlen(redi->file) == 0)
			break ;
		if (ft_strlen(line) != 0 && ft_strlen(line) == ft_strlen(redi->file) && \
			ft_strncmp(line, redi->file, ft_strlen(redi->file)) == 0)
			break ;
		ft_putstr_fd(line, cmd->read_fd);
		ft_putstr_fd("\n", cmd->read_fd);
	}
}

static void	reopen_here_doc(t_cmd *cmd)
{
	if (cmd->here_doc)
	{
		close(cmd->read_fd);
		cmd->read_fd = open(".temp", O_RDONLY);
		if (cmd->read_fd < 0)
			handle_systemcall_error();
	}
}

void	handle_here_doc(t_cmd *cmd)
{
	t_list	*node;
	t_redi	*redi;

	node = cmd->redi_list;
	while (node)
	{
		redi = node->content;
		if (ft_strncmp(redi->redi, "<<", 3) == 0)
		{
			cmd->here_doc = 1;
			if (cmd->read_fd != STDIN_FILENO)
				close(cmd->read_fd);
			cmd->read_fd = open(".temp", O_RDWR | O_CREAT | O_TRUNC, 0644);
			if (cmd->read_fd < 0)
				handle_systemcall_error();
			do_here_doc(cmd, redi);
		}
		node = node->next;
	}
	reopen_here_doc(cmd);
}

int	handle_redi(t_cmd *cmd)
{
	t_list	*node;
	t_redi	*redi;
	int		check;

	set_signal_heredoc();
	handle_here_doc(cmd);
	node = cmd->redi_list;
	while (node)
	{
		check = 0;
		redi = node->content;
		if (ft_strncmp(redi->redi, "<", 2) == 0)
			check = open_file(redi->file, O_RDONLY, READ, cmd);
		else if (ft_strncmp(redi->redi, ">", 2) == 0)
			check = open_file(redi->file, O_WRONLY | O_CREAT | O_TRUNC, \
				WRITE, cmd);
		else if (ft_strncmp(redi->redi, ">>", 2) == 0)
			check = open_file(redi->file, O_WRONLY | O_CREAT | O_APPEND, \
				WRITE, cmd);
		if (check == -1)
			return (-1);
		node = node->next;
	}
	return (0);
}
