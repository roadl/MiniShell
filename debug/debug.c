#include "minishell.h"

void	print_strs(char **strs)
{
	int	i;

	i = 0;
	if (strs)
		while (strs[i])
			printf("%s\n", strs[i++]);
}

void	print_cmd_list(t_list *lst)
{
	int		i;
	t_list	*node;
	t_cmd	*cmd;

	if (!lst)
	{
		printf("print_cmd_list: list null!\n");
		return ;
	}
	node = lst;
	i = 0;
	while (node)
	{
		cmd = node->content;
		printf("[%d]: %s\n", i++, cmd->cmd);
		node = node->next;
	}
}

void	print_envp(char **envp)
{
	int		i;

	if (!envp)
	{
		printf("print_envp: envp null!\n");
		return ;
	}
	i = 0;
	while (envp[i])
	{
		printf("[%d]: %s\n", i, envp[i]);
		i++;
	}
}
