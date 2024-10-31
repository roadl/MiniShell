/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_util.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yojin <yojin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:14:48 by yojin             #+#    #+#             */
/*   Updated: 2024/10/31 13:14:49 by yojin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static int	check_exit_range(char *exit_code)
{
	const char	*long_min = "9223372036854775808";
	const char	*long_max = "9223372036854775807";

	if (exit_code[0] == '-' && \
		ft_strncmp(exit_code + 1, long_min, ft_strlen(long_min)) > 0)
		return (0);
	if (exit_code[0] == '+')
		exit_code++;
	if (ft_strncmp(exit_code, long_max, ft_strlen(long_max)) > 0)
		return (0);
	return (1);
}

int	check_exit_numeric(char *arg)
{
	char		*exit_code;
	int			i;
	int			res;

	i = 0;
	res = 1;
	exit_code = ft_strtrim(arg, " ");
	if (!exit_code)
		handle_systemcall_error();
	if (exit_code[i] == '+' || exit_code[i] == '-')
		i = 1;
	if (!exit_code[i])
		res = 0;
	while (exit_code[i])
	{
		if (!ft_isdigit(exit_code[i++]))
			res = 0;
	}
	if (!check_exit_range(exit_code))
		res = 0;
	if (!res)
		free(exit_code);
	return (res);
}

int	check_echo_option(char *str)
{
	int	i;

	if (str && ft_strncmp(str, "-n", 2) == 0)
	{
		i = 2;
		while (str[i] && str[i] == 'n')
			i++;
		if (!str[i])
		{
			return (1);
		}
	}
	return (0);
}
