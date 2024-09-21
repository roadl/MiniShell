#include "minishell.h"

int	is_built_in(char *cmd)
{
	const char	built_in[7] = {"echo", "cd", "pwd", "export", \
		"unset", "env", "exit"};
	int			i;

	i = 0;
	while (i < 7)
	{
		if (ft_strncmp(built_in[i], cmd, ft_strlen(cmd)) == 0)
			return (1);
		i++;
	}
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

static void	free_cmd(t_cmd *cmd)
{
	int	i;

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
	free(cmd);
}

void	free_all(t_arg *arg)
{
	int	i;

	i = 0;
	if (arg->env_list)
		ft_lstclear(&arg->env_list, free);
	if (arg->cmd_list)
		ft_lstclear(&arg->cmd_list, free_cmd);
}
