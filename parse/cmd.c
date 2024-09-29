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

	node = arg->cmd_list;
	while (node)
	{
		cmd = node->content;
		if (cmd->cmd == NULL || is_built_in(cmd->cmd))
		{
			node = node->next;
			continue ;
		}
		if (ft_strchr(cmd->cmd, '/') == 0)
		{
			cmd_path = get_cmd(cmd->cmd, \
				get_env_value(find_env("PATH", arg->env_list)));
			if (!cmd_path)
				handle_systemcall_error();
			free(cmd->cmd);
			cmd->cmd = cmd_path;
		}
		node = node->next;
	}
}
