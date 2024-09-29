#include "minishell.h"

int	check_cd_path(char *path)
{
	struct stat	status;

	if (access(path, F_OK) != 0)
		return (print_error("cd", path, \
			strerror(ENOENT), error_built_in));
	if (stat(path, &status) == -1)
		handle_systemcall_error();
	if ((status.st_mode & S_IFMT) != S_IFDIR)
		return (print_error("cd", path, "Not a directory", error_built_in));
	if (access(path, X_OK) != 0)
		return (print_error("cd", path, "Permission denied", error_built_in));
	return (0);
}

int	check_cd_numeric(char *exit_code)
{
	const char	*long_min = "9223372036854775808";
	const char	*long_max = "9223372036854775807";
	int			i;

	i = 0;
	if (exit_code[i] == '+' || exit_code[i] == '-')
		i = 1;
	while (exit_code[i])
	{
		if (!ft_isdigit(exit_code[i]))
			return (0);
		i++;
	}
	if (exit_code[0] == '-' && \
		ft_strncmp(exit_code + 1, long_min, ft_strlen(long_min)) > 0)
		return (0);
	if (exit_code[0] == '+')
		exit_code++;
	if (ft_strncmp(exit_code, long_max, ft_strlen(long_max)) > 0)
		return (0);
	return (1);
}

int	check_echo_option(t_cmd *cmd)
{
	int	j;

	if (cmd->argv[1] && ft_strncmp(cmd->argv[1], "-n", 2) == 0)
	{
		j = 2;
		while (cmd->argv[1][j] && cmd->argv[1][j] == 'n')
			j++;
		if (!cmd->argv[1][j])
		{
			return (1);
		}
	}
	return (0);
}
