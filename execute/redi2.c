/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redi2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yojin <yojin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 16:04:04 by yojin             #+#    #+#             */
/*   Updated: 2024/10/31 16:04:05 by yojin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_heredoc_last(t_cmd *cmd)
{
	t_list	*node;
	t_redi	*redi;

	node = cmd->redi_list;
	while (node)
	{
		redi = node->content;
		if (!node->next && ft_strncmp(redi->redi, ">>", 3) == 0)
			return (1);
		node = node->next;
	}
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
		if ((ft_strlen(line) == 0 && ft_strlen(redi->file) == 0) || \
			(ft_strlen(line) != 0 && \
			ft_strlen(line) == ft_strlen(redi->file) && \
			ft_strncmp(line, redi->file, ft_strlen(redi->file)) == 0))
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, cmd->read_fd);
		ft_putstr_fd("\n", cmd->read_fd);
		free(line);
	}
}

static void	here_doc(t_cmd *cmd)
{
	t_list	*node;
	t_redi	*redi;

	set_signal_heredoc();
	node = cmd->redi_list;
	while (access(".temp_lock", F_OK) == 0)
		;
	if (open(".temp_lock", O_RDWR | O_CREAT | O_TRUNC, 0644) < 0)
		handle_systemcall_error();
	while (node)
	{
		redi = node->content;
		if (ft_strncmp(redi->redi, "<<", 3) == 0)
		{
			if (cmd->read_fd != STDIN_FILENO)
				close(cmd->read_fd);
			cmd->read_fd = open(".temp", O_RDWR | O_CREAT | O_TRUNC, 0644);
			if (cmd->read_fd < 0)
				handle_systemcall_error();
			do_here_doc(cmd, redi);
		}
		node = node->next;
	}
	unlink(".temp_lock");
}

static void	here_doc_parent(t_cmd *cmd)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		handle_systemcall_error();
	if (pid == 0)
	{
		here_doc(cmd);
		exit(EXIT_SUCCESS);
	}
	wait(0);
}

void	handle_here_doc(t_cmd *cmd)
{
	cmd->here_doc = check_heredoc_last(cmd);
	printf("is child %d\n", cmd->is_child);
	if (!cmd->is_child)
		here_doc_parent(cmd);
	else
		here_doc(cmd);
}
