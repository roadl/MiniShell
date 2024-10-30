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

int	is_input_error(char *input)
{
	int		i;
	char	*c;

	i = 0;
	while (input[i])
	{
		if (!is_allowed_char(input[i]))
		{
			c = ft_substr(input, i, 1);
			print_error("fastshell", NULL, c, error_syntax);
			free(c);
			return (1);
		}
		i++;
	}
	return (0);
}
