/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbel <jeongbel@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 04:17:15 by jeongbel          #+#    #+#             */
/*   Updated: 2024/10/31 19:11:06 by jeongbel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*allocate_cmds(int cmd_count)
{
	t_list	*cmds;
	t_cmd	*cmd;

	cmds = NULL;
	while (cmd_count--)
	{
		cmd = (t_cmd *)malloc(sizeof(t_cmd));
		if (!cmd)
			handle_systemcall_error();
		cmd->cmd = NULL;
		cmd->argv = NULL;
		cmd->read_fd = STDIN_FILENO;
		cmd->write_fd = STDOUT_FILENO;
		cmd->here_doc = 0;
		cmd->is_child = 0;
		cmd->redi_list = NULL;
		ft_lstadd_back(&cmds, ft_lstnew(cmd));
	}
	return (cmds);
}

void	store_redirection(t_list **redi_list, char **tokens, int *token_index)
{
	t_redi	*redi;

	redi = (t_redi *)malloc(sizeof(t_redi));
	if (!redi)
		handle_systemcall_error();
	redi->redi = ft_strdup(tokens[*token_index]);
	(*token_index)++;
	if (tokens[*token_index])
		redi->file = ft_strdup(tokens[*token_index]);
	else
		redi->file = ft_strdup("");
	if (!redi->redi || !redi->file)
		handle_systemcall_error();
	ft_lstadd_back(redi_list, ft_lstnew(redi));
}

void	tokens_to_cmd(char **tokens, t_cmd *cmd)
{
	int		argc;
	int		i;
	char	**cmd_tokens;

	if (!tokens || !(*tokens))
		return ;
	cmd_tokens = rm_redi_from_tokens(tokens);
	argc = ft_strslen(cmd_tokens);
	cmd->argv = (char **)malloc(sizeof(char *) * (argc + 1));
	if (!cmd->argv)
		handle_systemcall_error();
	i = 0;
	while (i < argc)
	{
		if (i == 0)
			cmd->cmd = ft_strdup(cmd_tokens[i]);
		cmd->argv[i] = ft_strdup(cmd_tokens[i]);
		if (!cmd->argv[i])
			handle_systemcall_error();
		i++;
	}
	cmd->argv[argc] = NULL;
	free_strs(cmd_tokens);
}

void	tokens_to_redi(char **tokens, t_cmd *cmd, int *i)
{
	t_list	*redi_list;

	redi_list = NULL;
	while (tokens[*i] && ft_strcmp(tokens[*i], "|"))
	{
		if (!ft_strcmp(tokens[*i], ">") || !ft_strcmp(tokens[*i], "<")
			|| !ft_strcmp(tokens[*i], ">>") || !ft_strcmp(tokens[*i], "<<"))
			store_redirection(&redi_list, tokens, i);
		(*i)++;
	}
	cmd->redi_list = redi_list;
}

t_list	*parsing(char *input, int *cmd_count)
{
	t_list	*cmds;
	char	**t;
	int		t_index;
	int		i;

	i = 0;
	if (is_blank(input))
		return (NULL);
	t = tokenize_input(input);
	if (!t)
		return (NULL);
	t_index = 0;
	*cmd_count = count_pipe(t) + 1;
	cmds = allocate_cmds(*cmd_count);
	while (t[t_index] && index_cmd(cmds, i))
	{
		tokens_to_cmd(&t[t_index], index_cmd(cmds, i));
		tokens_to_redi(t, index_cmd(cmds, i), &t_index);
		t_index++;
		i++;
	}
	free_strs(t);
	if (check_cmd_error(cmds, *cmd_count) || check_redi_error(cmds))
		return (NULL);
	return (cmds);
}
