#include "minishell.h"

static void	ft_putnstr_fd(char *s, int n, int fd)
{
	size_t	len;

	len = ft_strlen(s);
	if (n < len)
		len = n;
	write(fd, s, len);
}

static int	print_bi_error(char *cmd, char *arg, char *msg, t_error_type err_type)
{
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (err_type == invalid_identifier)
		ft_putstr_fd("`", STDERR_FILENO);
	if (err_type == invalid_option)
	{
		ft_putnstr_fd(arg, 2, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	else
		ft_putstr_fd(arg, STDERR_FILENO);
	if (err_type == invalid_identifier)
		ft_putstr_fd("'", STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	if (err_type == invalid_identifier)
		return (2);
	return (1);
}

static int	print_exec_error(char *cmd, char *arg, char *msg, t_error_type err_type)
{
	if (err_type == error_redirection)
		msg = strerror(errno);
	if (err_type != error_syntax && err_type != error_max_heredoc)
	{
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putstr_fd(msg, STDERR_FILENO);
	if (err_type == error_max_heredoc)
	{
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putstr_fd("\n", 2);
	if (err_type == error_redirection)
		exit(EXIT_FAILURE);
	if (err_type == error_max_heredoc)
		exit(2);
	return (1);
}

// 에러 났을때 STDERR_FILENO에 메세지 출력
// cmd: (arg: ) (msg or perror) 형식으로 출력
// arg 비어있으면 
int	print_error(char *cmd, char *arg, char *msg, t_error_type err_type)
{
	ft_putstr_fd("fastshell: ", STDERR_FILENO);
	if (err_type == error_systemcall)
	{
		perror("error");
		exit(EXIT_FAILURE);
	}
	if (err_type >= error_built_in)
		return (print_bi_error(cmd, arg, msg, err_type));
	return (print_exec_error(cmd, arg, msg, err_type));
}

void	free_all(t_arg *arg)
{
	int	i;

	i = 0;
	if (arg->env_list)
		ft_lstclear(&arg->env_list, free);
	if (arg->cmd_list)
		ft_lstclear(&arg->cmd_list, free);
}
