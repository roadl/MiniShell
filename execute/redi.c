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

void	handle_redi(t_cmd *cmd)
{
	t_list	*node;
	t_redi	*redi;

	cmd->read_fd = -1;
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
