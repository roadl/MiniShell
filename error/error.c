#include "minishell.h"

static void	ft_putnstr_fd(char *s, int n, int fd)
{
	int	len;

	len = ft_strlen(s);
	if (n < len)
		len = n;
	write(fd, s, len);
}

static int	print_bi_error(char *cmd, char *arg, char *msg, t_error_type type)
{
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (type == invalid_identifier)
		ft_putstr_fd("`", STDERR_FILENO);
	if (type == invalid_option)
		ft_putnstr_fd(arg, 2, STDERR_FILENO);
	else if (arg)
		ft_putstr_fd(arg, STDERR_FILENO);
	if (type == invalid_identifier)
		ft_putstr_fd("'", STDERR_FILENO);
	if (arg)
		ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	if (type == invalid_identifier)
		return (2);
	return (1);
}

static int	print_exec_error(char *arg, char *msg, t_error_type type)
{
	if (type == error_redirection)
		msg = strerror(errno);
	if (type == error_syntax)
	{
		ft_putstr_fd("parse error near unexpected token `", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd("'", STDERR_FILENO);
	}
	if (type != error_syntax && type != error_max_heredoc)
	{
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putstr_fd(msg, STDERR_FILENO);
	if (type == error_max_heredoc)
	{
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putstr_fd("\n", STDERR_FILENO);
	if (type == error_redirection)
		exit(EXIT_FAILURE);
	if (type == error_max_heredoc)
		exit(2);
	return (1);
}

// 에러 났을때 STDERR_FILENO에 메세지 출력
// cmd: (arg: ) msg 형식으로 출력
int	print_error(char *cmd, char *arg, char *msg, t_error_type err_type)
{
	ft_putstr_fd("fastshell: ", STDERR_FILENO);
	if (err_type == error_systemcall)
	{
		perror("error");
		exit(EXIT_FAILURE);
	}
    if (err_type == error_syntax && !arg)
    {
        ft_putstr_fd("syntax error near unexpected token `", STDERR_FILENO);
        ft_putstr_fd(msg, STDERR_FILENO);
        ft_putstr_fd("'\n", STDERR_FILENO);
        return (2);
    }
	if (err_type >= error_built_in && err_type <= invalid_option)
		return (print_bi_error(cmd, arg, msg, err_type));
	return (print_exec_error(arg, msg, err_type));
}

void	handle_systemcall_error(void)
{
	print_error(NULL, NULL, NULL, error_systemcall);
}
