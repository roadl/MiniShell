#include "minishell.h"

int	is_built_in(char *cmd)
{
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

// 아니 슈발 왜 redi에 NULL이 안 들어가 있을때가 있지 ㅈ버근가
void	free_cmd(void *_cmd)
{
	t_cmd	*cmd;
	int		i;

	cmd = _cmd;
	// printf("free cmd: %p\n", cmd->cmd);
	free(cmd->cmd);
	i = 0;
	if (cmd->argv)
	{
		while (cmd->argv[i])
		{
			// printf("free argv[%d]: %p\n", i, cmd->argv[i]);
			free(cmd->argv[i]);
			i++;
		}
		// printf("free argv: %p\n", cmd->argv);
		free(cmd->argv);
	}
	// printf("free redi_list: %p\n", cmd->redi_list);
	// if (cmd->redi_list)
		// ft_lstclear(&cmd->redi_list, free);
	// printf("free: %p\n", cmd);
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
