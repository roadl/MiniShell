/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redi.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yojin <yojin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:15:16 by yojin             #+#    #+#             */
/*   Updated: 2024/10/31 17:17:41 by yojin            ###   ########.fr       */
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

static void	reopen_here_doc(t_cmd *cmd)
{
	if (cmd->here_doc)
	{
		close(cmd->read_fd);
		cmd->read_fd = open(HEREDOC, O_RDONLY);
		if (cmd->read_fd < 0)
			handle_systemcall_error();
	}
}

int	handle_redi(t_cmd *cmd)
{
	t_list	*node;
	t_redi	*redi;
	int		check;

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
		else if (ft_strncmp(redi->redi, ">>", 3) == 0)
			check = open_file(redi->file, O_WRONLY | O_CREAT | O_APPEND, \
				WRITE, cmd);
		if (check == -1)
			return (-1);
		node = node->next;
	}
	reopen_here_doc(cmd);
	return (0);
}
