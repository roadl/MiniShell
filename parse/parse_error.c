/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbel <jeongbel@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 06:10:49 by jeongbel          #+#    #+#             */
/*   Updated: 2024/10/31 12:23:42 by jeongbel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_cmd_error(t_list *cmd_list, int cmd_count)
{
	if (cmd_count != 0 && is_cmd_empty(index_cmd(cmd_list, cmd_count - 1)))
	{
		print_error("fastshell", NULL, "|", error_syntax);
		ft_lstclear(&cmd_list, free_cmd);
		return (1);
	}
	return (0);
}

int	check_redi_error(t_list *cmd_list)
{
	t_list	*node;
	t_cmd	*cmd;

	node = cmd_list;
	while (node)
	{
		cmd = node->content;
		if (is_redi_error(cmd))
		{
			ft_lstclear(&cmd_list, free_cmd);
			return (1);
		}
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
		if (input[i] == '\'' || input[i] == '"')
			handle_quotes(input, &i, input[i]);
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
