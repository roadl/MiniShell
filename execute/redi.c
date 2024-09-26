#include "minishell.h"

#define READ 0
#define WRITE 1

static void	open_file(char *file, int option, int type, int *_fd)
{
	int	fd;

	fd = *_fd;
	if (fd != -1)
		close(fd);
	if (type == READ)
		fd = open(file, option);
	else
		fd = open(file, option, 0644);
	if (fd < 0)
		handle_systemcall_error();
	*_fd = fd;
}

void	handle_here_doc(t_cmd *cmd)
{
	t_list	*node;
	t_redi	*redi;
	char	*line;

	cmd->read_fd = -1;
	node = cmd->redi_list;
	while (node)
	{
		redi = node->content;
		if (ft_strncmp(redi->redi, "<<", 3) == 0)
		{
			cmd->read_fd = open(".temp", O_RDWR | O_CREAT | O_TRUNC, 0644);
			if (cmd->read_fd < 0)
				handle_systemcall_error();
			while (1)
			{
				line = readline(">");
				if (ft_strncmp(line, redi->file, ft_strlen(line)) != 0)
					break ;
				ft_putstr_fd(line, cmd->read_fd);
			}
			close(cmd->read_fd);
		}
		node = node->next;
	}
}

void	handle_redi(t_cmd *cmd)
{
	t_list	*node;
	t_redi	*redi;

	cmd->read_fd = -1;
	handle_here_doc(cmd);
	cmd->write_fd = -1;
	node = cmd->redi_list;
	while (node)
	{
		redi = node->content;
		if (ft_strncmp(redi->redi, "<", 2) == 0)
			open_file(redi->file, O_RDONLY, READ, &cmd->read_fd);
		else if (ft_strncmp(redi->redi, ">", 2) == 0)
			open_file(redi->file, O_WRONLY | O_CREAT | O_TRUNC, \
				WRITE, &cmd->write_fd);
		else if (ft_strncmp(redi->redi, ">>", 2) == 0)
			open_file(redi->file, O_WRONLY | O_CREAT, WRITE, &cmd->write_fd);
		node = node->next;
	}
	if (cmd->read_fd == -1)
		cmd->read_fd = STDIN_FILENO;
	if (cmd->write_fd == -1)
		cmd->write_fd = STDOUT_FILENO;
}
