/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbel <jeongbel@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 06:10:53 by jeongbel          #+#    #+#             */
/*   Updated: 2024/10/31 06:10:54 by jeongbel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_cmd(char *cmd, char *path)
{
	char	*cmd_path;
	char	**path_list;
	char	*cmd_with_path;
	int		i;

	path_list = ft_split(path, ':');
	if (!*cmd)
		return (ft_strdup(cmd));
	i = 0;
	while (path_list[i])
	{
		cmd_path = ft_strjoin(path_list[i], "/");
		cmd_with_path = ft_strjoin(cmd_path, cmd);
		if (!access(cmd_with_path, X_OK))
		{
			free(cmd_path);
			free_strs(path_list);
			return (cmd_with_path);
		}
		free(cmd_path);
		free(cmd_with_path);
		i++;
	}
	free_strs(path_list);
	return (ft_strdup(cmd));
}

void	make_cmd(t_arg *arg)
{
	t_list	*node;
	t_cmd	*cmd;
	char	*cmd_path;
	char	*path;

	node = arg->cmd_list;
	while (node)
	{
		cmd = node->content;
		if (!(cmd->cmd == NULL || is_built_in(cmd->cmd))
			&& ft_strchr(cmd->cmd, '/') == 0)
		{
			path = get_env_value(find_env("PATH", arg->env_list));
			if (path)
			{
				cmd_path = get_cmd(cmd->cmd, path);
				free(path);
				if (!cmd_path)
					handle_systemcall_error();
				free(cmd->cmd);
				cmd->cmd = cmd_path;
			}
		}
		node = node->next;
	}
}
