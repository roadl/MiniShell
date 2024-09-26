#include "minishell.h"

// 토큰화 함수
char	**tokenize_input(char *input)
{
	char **tokens;
	int count = 0;
	int i = 0, j = 0, start = 0;
	int quote = 0;

	// 메모리 할당 (최대 input 길이만큼의 포인터 배열)
	tokens = (char **)malloc(sizeof(char *) * (ft_strlen(input) + 1));
	if (!tokens)
		return NULL;
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '"')
		{
			quote = input[i];
			i++;
			// 이 while 문에서 끝나면 syntax error
			while (input[i] && input[i] != quote)
				i++;
		}
		if (input[i] == ' ' || input[i] == '\t' || input[i] == '|' || input[i] == '>' || input[i] == '<')
		{
			if (i > start)
			{
				tokens[count] = (char *)malloc(i - start + 1);
				strncpy(tokens[count], &input[start], i - start);
				tokens[count][i - start] = '\0';
				count++;
			}
			if ((input[i] == '>' && input[i + 1] == '>') || (input[i] == '<' && input[i + 1] == '<'))
			{
				tokens[count] = (char *)malloc(3);
				tokens[count][0] = input[i];
				tokens[count][1] = input[i + 1];
				tokens[count][2] = '\0';
				i++;
				count++;
			}
			else if (input[i] == '>' || input[i] == '<' || input[i] == '|')
			{
				tokens[count] = (char *)malloc(2);
				tokens[count][0] = input[i];
				tokens[count][1] = '\0';
				count++;
			}
			else if (input[i] == '|')
			{
				tokens[count] = (char *)malloc(2);
				tokens[count][0] = '|';
				tokens[count][1] = '\0';
				count++;
			}
			start = i + 1;
		}
		i++;
	}
	if (i > start)
	{
		tokens[count] = (char *)malloc(i - start + 1);
		strncpy(tokens[count], &input[start], i - start);
		tokens[count][i - start] = '\0';
		count++;
	}
	tokens[count] = NULL;
	return tokens;
}

t_cmd	*parse_pipeline(char *input, int *cmd_count)
{
	char **pipe_segments;
	t_cmd *cmds;
	int i, j;

	// 파이프 단위로 분할
	pipe_segments = ft_split(input, '|');
	*cmd_count = 0;
	while (pipe_segments[*cmd_count])
		(*cmd_count)++;
	printf("cmd_count: %d\n", *cmd_count);

	// t_cmd 구조체 배열 할당
	cmds = (t_cmd *)malloc(sizeof(t_cmd) * (*cmd_count));
	if (!cmds)
		return NULL;

	for (i = 0; i < *cmd_count; i++)
	{
		cmds[i].redi_list = NULL;
		cmds[i].cmd = NULL;
		cmds[i].argv = NULL;

		char **tokens = tokenize_input(pipe_segments[i]);
		t_list *redi_list = NULL;
		t_list *last_redi = NULL;
		int token_index = 0;

		// 리디렉션 처리
		while (tokens[token_index])
		{
			if (strcmp(tokens[token_index], ">") == 0 || strcmp(tokens[token_index], "<") == 0 ||
				strcmp(tokens[token_index], ">>") == 0 || strcmp(tokens[token_index], "<<") == 0)
			{
				printf("token_index: %d\n", token_index);
				t_redi *redi = (t_redi *)malloc(sizeof(t_redi));
				redi->redi = ft_strdup(tokens[token_index]);
				tokens = remove_str_from_array(tokens, token_index);
				redi->file = ft_strdup(tokens[token_index]);
				tokens = remove_str_from_array(tokens, token_index);
				token_index--;
				t_list *new_node = ft_lstnew(redi);
				if (!redi_list)
					redi_list = new_node;
				else
					last_redi->next = new_node;
				last_redi = new_node;
			}
			token_index++;
		}
		cmds[i].redi_list = redi_list;

		token_index = 0;
		// 명령어와 인자 처리
		int argc = 0;
		for (j = token_index; tokens[j]; j++)
			argc++;
		cmds[i].argv = (char **)malloc(sizeof(char *) * (argc + 1));
		for (j = token_index; tokens[j]; j++)
		{
			if (j == token_index)
				cmds[i].cmd = strdup(tokens[j]);
			cmds[i].argv[j - token_index] = strdup(tokens[j]);
		}
		cmds[i].argv[argc] = NULL;

		// 메모리 해제
		for (j = 0; tokens[j]; j++)
			free(tokens[j]);
		free(tokens);
	}

	// 메모리 해제
	for (i = 0; pipe_segments[i]; i++)
		free(pipe_segments[i]);
	free(pipe_segments);

	return cmds;
}

// // test main 함수
// int main(void)
// {
// 	char *input;
// 	t_cmd *cmds;
// 	int cmd_count, i, j;

// 	input = readline("minishell> ");
// 	if (!input)
// 		return 1;
// 	add_history(input);

// 	cmds = parse_pipeline(input, &cmd_count);
// 	for (i = 0; i < cmd_count; i++)
// 	{
// 		printf("Command[%d]: %s\n", i, cmds[i].cmd);
// 		for (j = 0; cmds[i].argv[j]; j++)
// 		{
// 			printf("Arg[%d][%d]: %s\n", i, j, cmds[i].argv[j]);
// 		}
// 		t_list *redi = cmds[i].redi_list;
// 		while (redi)
// 		{
// 			t_redi *r = (t_redi *)redi->content;
// 			printf("Redirection[%d]: %s %s\n", i++, r->redi, r->file);
// 			redi = redi->next;
// 		}
// 	}

// 	// 메모리 해제
// 	for (i = 0; i < cmd_count; i++)
// 	{
// 		free(cmds[i].cmd);
// 		for (j = 0; cmds[i].argv[j]; j++)
// 			free(cmds[i].argv[j]);
// 		free(cmds[i].argv);
// 		t_list *redi = cmds[i].redi_list;
// 		while (redi)
// 		{
// 			t_list *next = redi->next;
// 			t_redi *r = (t_redi *)redi->content;
// 			free(r->redi);
// 			free(r->file);
// 			free(r);
// 			free(redi);
// 			redi = next;
// 		}
// 	}
// 	free(cmds);
// 	free(input);

// 	return 0;
// }