#include "minishell.h"

t_list	*allocate_cmds(int cmd_count)
{
	t_list	*cmds;
	t_cmd	*cmd;

	cmds = NULL;
	while (cmd_count--)
	{
		cmd = (t_cmd *)malloc(sizeof(t_cmd));
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
	redi->redi = ft_strdup(tokens[*token_index]);
	(*token_index)++;
	redi->file = ft_strdup(tokens[*token_index]);
	ft_lstadd_back(redi_list, ft_lstnew(redi));
}

void	process_tokens(char **tokens, t_cmd *cmd)
{
	int	argc;
	int	i;

	argc = 0;
	while (tokens[argc])
		argc++;
	cmd->argv = (char **)malloc(sizeof(char *) * (argc + 1));
	i = 0;
	while (i < argc)
	{
		if (i == 0)
			cmd->cmd = strdup(tokens[i]);
		cmd->argv[i] = strdup(tokens[i]);
		i++;
	}
	cmd->argv[argc] = NULL;
}

t_list *parsing(char *input, int *cmd_count)
{
	char	**pipe_segments;
	t_list	*cmds;
	
	pipe_segments = ft_split(input, '|');
	*cmd_count = 0;
	while (pipe_segments[*cmd_count])
		(*cmd_count)++;
	cmds = allocate_cmds(*cmd_count);
	for (int i = 0; i < *cmd_count; i++)
	{
		char **tokens = tokenize_input(pipe_segments[i]);
		t_list *redi_list = NULL;
		int token_index = 0;

		if (!tokens)
		{
			ft_lstclear(&cmds, free_cmd);
			return NULL;
		}
		while (tokens[token_index])
		{
			if (strcmp(tokens[token_index], ">") == 0 || strcmp(tokens[token_index], "<") == 0 ||
				strcmp(tokens[token_index], ">>") == 0 || strcmp(tokens[token_index], "<<") == 0)
			{
				store_redirection(&redi_list, tokens, &token_index);
				tokens = remove_str_from_array(tokens, --token_index);
				tokens = remove_str_from_array(tokens, token_index--);
			}
			token_index++;
		}
		t_cmd *cmd = (t_cmd *)index_cmd(cmds, i);
		cmd->read_fd = STDIN_FILENO;
		cmd->write_fd = STDOUT_FILENO;
		cmd->redi_list = redi_list;
		process_tokens(tokens, cmd);
		for (int j = 0; tokens[j]; j++)
			free(tokens[j]);
		free(tokens);
	}
	for (int i = 0; pipe_segments[i]; i++)
		free(pipe_segments[i]);
	free(pipe_segments);

	return cmds;
}

// // test main 함수
// int main(void)
// {
// 	char *input;
// 	t_list *cmds;
// 	int cmd_count, i, j;

// 	input = readline("minishell> ");
// 	if (!input)
// 		return 1;
// 	add_history(input);

// 	cmds = parsing(input, &cmd_count);
// 	printf("cmd_count: %d\n", cmd_count);
// 	for (i = 0; i < cmd_count; i++)
// 	{
// 		t_cmd *cmd = index_cmd(cmds, i);
// 		printf("cmd: %s\n", cmd->cmd);
// 		for (j = 0; cmd->argv[j]; j++)
// 			printf("argv[%d][%d]: %s\n", i, j, cmd->argv[j]);
// 		printf("\n");
// 		t_list *node = cmd->redi_list;
// 		while (node)
// 		{
// 			t_redi *redi = node->content;
// 			printf("redi: %s\n", redi->redi);
// 			printf("file: %s\n", redi->file);
// 			node = node->next;
// 		}
// 	}

// 	// 메모리 해제
// 	ft_lstclear(&cmds, free_cmd);
// 	free(input);

// 	return 0;
// }
