/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yojin <yojin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 16:04:13 by yojin             #+#    #+#             */
/*   Updated: 2024/10/31 18:55:31 by yojin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define READ 0
#define WRITE 1

int	g_exit_code = 0;

void	set_signal_status(void)
{
	g_exit_code = 1;
}

void	unlink_tmp_file(void)
{
	if (unlink(HEREDOC) == -1 && errno != ENOENT)
		perror("fastshell: ");
	unlink(HEREDOC_LOCK);
}

void	process_line(char *line, t_arg *arg)
{
	int	cmd_count;

	unlink_tmp_file();
	arg->cmd_list = parsing(line, &cmd_count);
	if (!arg->cmd_list)
	{
		arg->last_exit_code = 258;
		return ;
	}
	process_quotes(arg);
	make_cmd(arg);
	if (cmd_count == 1 && is_cmd_empty(index_cmd(arg->cmd_list, 0)))
	{
		ft_lstclear(&arg->cmd_list, free_cmd);
		return ;
	}
	print_cmd_list(arg->cmd_list);
	set_signal_fork();
	g_exit_code = exec_cmds(arg);
	ft_lstclear(&arg->cmd_list, free_cmd);
	unlink_tmp_file();
}

// 들어올때 SHLVL 하나 증가시켜야 함
int	main(int argc, char **argv, char **envp)
{
	char			*line;
	t_arg			arg;

	(void)argv;
	if (argc != 1 || !init_arg(&arg, envp))
		exit(EXIT_FAILURE);
	while (1)
	{
		set_terminal_print_off();
		set_signal();
		line = readline("FastSHELL$ ");
		if (line == NULL)
		{
			do_sigterm();
			exit(EXIT_SUCCESS);
		}
		if (*line == '\0')
		{
			free(line);
			continue ;
		}
		add_history(line);
		process_line(line, &arg);
		free(line);
	}
}
