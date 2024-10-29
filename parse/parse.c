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
	int	argc;
	int	i;

	if (!tokens || !(*tokens))
		return ;
	argc = 0;
	while (tokens[argc])
		argc++;
	cmd->argv = (char **)malloc(sizeof(char *) * (argc + 1));
	if (!cmd->argv)
		handle_systemcall_error();
	i = 0;
	while (i < argc)
	{
		if (i == 0)
			cmd->cmd = ft_strdup(tokens[i]);
		cmd->argv[i] = ft_strdup(tokens[i]);
		if (!cmd->argv[i])
			handle_systemcall_error();
		i++;
	}
	cmd->argv[argc] = NULL;
}

t_list  *parsing(char *input, int *cmd_count)
{
	char	**pipe_segments;
	t_list	*cmds;
	char	**tokens;
	t_list	*redi_list;

	pipe_segments = ft_split(input, '|');
	*cmd_count = count_pipe(input) + 1;
	cmds = allocate_cmds(*cmd_count);
	for (int i = 0; i < *cmd_count; i++)
	{
		tokens = tokenize_input(pipe_segments[i]);
		redi_list = NULL;
		int token_index = 0;

		if (!tokens || !(*tokens))
			break ;
		while (tokens[token_index])
		{
			if (strcmp(tokens[token_index], ">") == 0 || strcmp(tokens[token_index], "<") == 0
			||	strcmp(tokens[token_index], ">>") == 0 || strcmp(tokens[token_index], "<<") == 0)
			{
				store_redirection(&redi_list, tokens, &token_index);
				tokens = remove_str_from_array(tokens, --token_index);
				tokens = remove_str_from_array(tokens, token_index--);
			}
			token_index++;
		}
		t_cmd *cmd = (t_cmd *)index_cmd(cmds, i);
		cmd->redi_list = redi_list;
		process_tokens(tokens, cmd);
		free_strs(tokens);
	}
	free_strs(pipe_segments);
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
