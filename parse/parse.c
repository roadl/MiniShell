#include "minishell.h"

t_list *allocate_cmds(int cmd_count)
{
	t_list *cmds = NULL;

	for (int i = 0; i < cmd_count; i++)
	{
		t_cmd *cmd = (t_cmd *)malloc(sizeof(t_cmd));
		cmd->cmd = NULL;
		cmd->argv = NULL;
		cmd->read_fd = -1;
		cmd->write_fd = -1;
		cmd->redi_list = NULL;
		t_list *new_node = ft_lstnew(cmd);
		ft_lstadd_back(&cmds, new_node);
	}
	return cmds;
}

void store_redirection(t_list **redi_list, t_list **last_redi, char **tokens, int *token_index)
{
	t_redi *redi = (t_redi *)malloc(sizeof(t_redi));
	redi->redi = ft_strdup(tokens[*token_index]);
	(*token_index)++;
	redi->file = ft_strdup(tokens[*token_index]);
	t_list *new_node = ft_lstnew(redi);
	if (!(*redi_list))
		*redi_list = new_node;
	else
		(*last_redi)->next = new_node;
	*last_redi = new_node;
}

void process_tokens(char **tokens, t_cmd *cmd)
{
	int token_index = 0;
	int argc = 0;
	while (tokens[argc])
		argc++;
	cmd->argv = (char **)malloc(sizeof(char *) * (argc + 1));
	for (int j = 0; tokens[j]; j++)
	{
		if (j == token_index)
			cmd->cmd = strdup(tokens[j]);
		cmd->argv[j - token_index] = strdup(tokens[j]);
	}
	cmd->argv[argc] = NULL;
}

t_list *parsing(char *input, int *cmd_count)
{
	char **pipe_segments = ft_split(input, '|');
	*cmd_count = 0;
	while (pipe_segments[*cmd_count])
		(*cmd_count)++;
	t_list *cmds = allocate_cmds(*cmd_count);
	for (int i = 0; i < *cmd_count; i++)
	{
		char **tokens = tokenize_input(pipe_segments[i]);
		t_list *redi_list = NULL, *last_redi = NULL;
		int token_index = 0;

		while (tokens[token_index])
		{
			if (strcmp(tokens[token_index], ">") == 0 || strcmp(tokens[token_index], "<") == 0 ||
				strcmp(tokens[token_index], ">>") == 0 || strcmp(tokens[token_index], "<<") == 0)
			{
				store_redirection(&redi_list, &last_redi, tokens, &token_index);
				tokens = remove_str_from_array(tokens, --token_index);
				tokens = remove_str_from_array(tokens, token_index--);
			}
			token_index++;
		}
		t_cmd *cmd = (t_cmd *)index_cmd(cmds, i);
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
// 		printf("argv: ");
// 		for (j = 0; cmd->argv[j]; j++)
// 			printf("%s ", cmd->argv[j]);
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
