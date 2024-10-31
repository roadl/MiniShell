/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yojin <yojin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 15:05:58 by yojin             #+#    #+#             */
/*   Updated: 2024/10/31 15:11:16 by yojin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_built_in(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (1);
	if (ft_strncmp(cmd, "cd", 3) == 0)
		return (1);
	if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "export", 7) == 0)
		return (1);
	if (ft_strncmp(cmd, "unset", 6) == 0)
		return (1);
	if (ft_strncmp(cmd, "env", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	return (0);
}

int	is_only_built_in(t_arg *arg)
{
	t_cmd	*cmd;

	cmd = arg->cmd_list->content;
	if (ft_lstsize(arg->cmd_list) == 1 && is_built_in(cmd->cmd))
		return (1);
	return (0);
}

void	free_strs(void *_strs)
{
	char	**strs;
	int		i;

	strs = _strs;
	i = 0;
	if (strs)
		while (strs[i])
			free(strs[i++]);
	free(strs);
}

void	free_redi(void *_redi)
{
	t_redi	*redi;

	redi = _redi;
	free(redi->redi);
	free(redi->file);
	free(redi);
}

void	free_cmd(void *_cmd)
{
	t_cmd	*cmd;
	int		i;

	cmd = _cmd;
	free(cmd->cmd);
	i = 0;
	if (cmd->argv)
	{
		while (cmd->argv[i])
		{
			free(cmd->argv[i]);
			i++;
		}
		free(cmd->argv);
	}
	if (cmd->redi_list)
		ft_lstclear(&cmd->redi_list, free_redi);
	free(cmd);
}
