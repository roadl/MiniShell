/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbel <jeongbel@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 04:17:15 by jeongbel          #+#    #+#             */
/*   Updated: 2024/10/31 06:29:13 by jeongbel         ###   ########.fr       */
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

void	process_tokens(char **tokens, t_cmd *cmd)
{
	int		argc;
	int		i;
	char	**cmd_tokens;

	if (!tokens || !(*tokens))
		return ;
	cmd_tokens = rm_redi_from_tokens(tokens);
	argc = 0;
	while (cmd_tokens[argc])
		argc++;
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

char	**rm_redi_from_tokens(char **tokens)
{
	char	**new_tokens;
	int		i;
	int		j;

	new_tokens = (char **)malloc(sizeof(char *) * (ft_strslen(tokens) + 1));
	if (!new_tokens)
		handle_systemcall_error();
	i = 0;
	j = 0;
	while (tokens[i] && ft_strcmp(tokens[i], "|"))
	{
		if (!ft_strcmp(tokens[i], ">") || !ft_strcmp(tokens[i], ">>")
			|| !ft_strcmp(tokens[i], "<") || !ft_strcmp(tokens[i], "<<"))
		{
			i += 2;
			continue ;
		}
		new_tokens[j] = ft_strdup(tokens[i]);
		if (!new_tokens[j])
			handle_systemcall_error();
		i++;
		j++;
	}
	new_tokens[j] = NULL;
	return (new_tokens);
}

t_list	*parsing(char *input, int *cmd_count)
{
	char	**seg;
	t_list	*cmds;
	char	**tokens;
	t_list	*redi_list;
	int		token_index;
	int		i;

	i = 0;
	seg = tokenize_input(input);
	if (!seg)
		return (NULL);
	token_index = 0;
	*cmd_count = count_pipe(seg) + 1;
	cmds = allocate_cmds(*cmd_count);
	while (seg[token_index])
	{
		tokens = &seg[token_index];
		redi_list = NULL;
		while (ft_strcmp(seg[token_index], "|") && seg[token_index])
		{
			if (!ft_strcmp(seg[token_index], ">") || !ft_strcmp(seg[token_index], "<")
				|| !ft_strcmp(seg[token_index], ">>") || !ft_strcmp(seg[token_index], "<<"))
				store_redirection(&redi_list, seg, &token_index);
			token_index++;
		}
		token_index++;
		index_cmd(cmds, i)->redi_list = redi_list;
		process_tokens(tokens, index_cmd(cmds, i));
		i++;
	}
	free_strs(seg);
	if (*cmd_count != 1 && is_cmd_empty(index_cmd(cmds, *cmd_count - 1)))
	{
		ft_lstclear(&cmds, free_cmd);
		print_error("fastshell", NULL, "|", error_syntax);
		return (NULL);
	}
	if (check_redi_error(cmds))
	{
		ft_lstclear(&cmds, free_cmd);
		return (NULL);
	}
	return (cmds);
}
