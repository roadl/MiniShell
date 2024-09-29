#include "minishell.h"

int	is_allowed_char(char c)
{
	if (ft_isalnum(c) || c == '_' || c == '<' || c == '>' || c == '.'
		|| c == '|' || c == '"' || c == '\'' || c == '$' || c == '-'
		|| c == '/' || c == ' ' || c == '\t' || c == '=' || c == '~'
		|| c == '?')
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

int	is_redi_error(t_cmd *cmd)
{
	t_list	*node;
	t_redi	*redi;

	node = cmd->redi_list;
	while (node)
	{
		redi = node->content;
		if (!redi->file || !ft_strlen(redi->file))
		{
			print_error("fastshell", NULL, "\\n", error_syntax);
			return (1);
		}
		else if (!ft_strcmp(redi->file, ">") || !ft_strcmp(redi->file, ">>")
			|| !ft_strcmp(redi->file, "<") || !ft_strcmp(redi->file, "<<"))
		{
			print_error("fastshell", NULL, redi->redi, error_syntax);
			return (1);
		}
		node = node->next;
	}
	return (0);
}
