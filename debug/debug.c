#include "minishell.h"

void	print_list(t_list *lst)
{
	int		i;
	t_list	*node;

	if (!lst)
	{
		printf("print_list: list null!\n");
		return ;
	}
	node = lst;
	i = 0;
	while (node)
	{
		printf("[%d]: %s\n", i++, node->content);
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
