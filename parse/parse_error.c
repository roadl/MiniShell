#include "minishell.h"

int	check_redi_error(t_list *cmd_list)
{
	t_list	*node;
	t_cmd	*cmd;

	node = cmd_list;
	while (node)
	{
		cmd = node->content;
		if (is_redi_error(cmd))
			return (1);
		node = node->next;
	}
	return (0);
}