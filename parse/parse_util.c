/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yojin <yojin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 06:10:45 by jeongbel          #+#    #+#             */
/*   Updated: 2024/10/31 13:45:07 by yojin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_allowed_char(char c)
{
	if (ft_isalnum(c) || c == '_' || c == '<' || c == '>' || c == '.'
		|| c == '|' || c == '"' || c == '\'' || c == '$' || c == '-'
		|| c == '/' || c == ' ' || c == '\t' || c == '=' || c == '~'
		|| c == '?' || c == '+')
		return (1);
	return (0);
}

int	count_pipe(char **input)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	if (!input)
		return (0);
	while (input[i])
	{
		if (ft_strcmp(input[i], "|") == 0)
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

t_token_state	*init_token_state(void)
{
	t_token_state	*state;

	state = (t_token_state *)malloc(sizeof(t_token_state));
	if (!state)
		handle_systemcall_error();
	state->tokens = NULL;
	state->count = 0;
	state->i = 0;
	state->start = 0;
	return (state);
}
