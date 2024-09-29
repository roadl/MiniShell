#include "minishell.h"

int	is_allowed_char(char c)
{
	if (ft_isalnum(c) || c == '_' || c == '<' || c == '>' || c == '.'
		|| c == '|' || c == '"' || c == '\'' || c == '$' || c == '-' || c == '/'
			|| c == ' ' || c == '\t' || c == '=' || c == '~' || c == '?')
		return (1);
	return (0);
}

int	count_pipe(char *input)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	if (!input)
		return (0);
	while (input[i])
	{
		if (input[i] == '|')
			count++;
		i++;
	}
	return (count);
}

int	is_cmd_empty(t_cmd *cmd)
{
	if (!cmd->cmd && !cmd->argv && !cmd->redi_list)
		return (1);
	return (0);
}
