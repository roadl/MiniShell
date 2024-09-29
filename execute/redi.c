#include "minishell.h"

#define READ 0
#define WRITE 1

// static void	print_fd_status(int fd)
// {
// 	int flags = fcntl(fd, F_GETFL);

// 	printf("fd: %d: ", fd);
//     if (flags == -1) {
//         printf("Error getting flags\n");
//         return ;
//     }

//     // 상태 출력
//     if (flags & O_RDONLY) {
//         printf("The file descriptor is opened in read-only mode.\n");
//     }
//     if (flags & O_WRONLY) {
//         printf("The file descriptor is opened in write-only mode.\n");
//     }
//     if (flags & O_RDWR) {
//         printf("The file descriptor is opened in read-write mode.\n");
//     }
// }

static void	open_file(char *file, int option, int type, t_cmd *cmd)
{
	if (type == READ && cmd->read_fd != STDIN_FILENO)
		close(cmd->read_fd);
	if (type == WRITE && cmd->write_fd != STDOUT_FILENO)
		close(cmd->write_fd);
	if (type == READ)
		cmd->read_fd = open(file, option);
	else
		cmd->write_fd = open(file, option, 0644);
	if (cmd->read_fd < 0 || cmd->write_fd < 0)
		handle_systemcall_error();
}

void	handle_here_doc(t_cmd *cmd)
{
	t_list	*node;
	t_redi	*redi;
	char	*line;

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
			while (1)
			{
				line = readline("> ");
				if (ft_strncmp(line, redi->file, ft_strlen(line)) == 0)
					break ;
				ft_putstr_fd(line, cmd->read_fd);
				ft_putstr_fd("\n", cmd->read_fd);
			}
		}
		node = node->next;
	}
}

void	handle_redi(t_cmd *cmd)
{
	t_list	*node;
	t_redi	*redi;

	set_signal_heredoc();
	handle_here_doc(cmd);
	node = cmd->redi_list;
	if (cmd->here_doc)
	{
		close(cmd->read_fd);
		cmd->read_fd = open(".temp", O_RDONLY);
		if (cmd->read_fd < 0)
			handle_systemcall_error();
	}
	while (node)
	{
		redi = node->content;
		if (ft_strncmp(redi->redi, "<", 2) == 0)
			open_file(redi->file, O_RDONLY, READ, cmd);
		else if (ft_strncmp(redi->redi, ">", 2) == 0)
			open_file(redi->file, O_WRONLY | O_CREAT | O_TRUNC, \
				WRITE, cmd);
		else if (ft_strncmp(redi->redi, ">>", 2) == 0)
			open_file(redi->file, O_WRONLY | O_CREAT | O_APPEND, WRITE, cmd);
		node = node->next;
	}
	if (cmd->read_fd == -1)
		cmd->read_fd = STDIN_FILENO;
	if (cmd->write_fd == -1)
		cmd->write_fd = STDOUT_FILENO;
}
