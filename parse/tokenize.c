#include "minishell.h"

char	**allocate_tokens(char *input)
{
	char	**tokens;

	tokens = (char **)malloc(sizeof(char *) * (ft_strlen(input) + 1));
	if (!tokens)
		return (NULL);
	return (tokens);
}

void	handle_quotes(char *input, int *i, char quote)
{
	(*i)++;
	while (input[*i] && input[*i] != quote)
		(*i)++;
}

char	**store_token(char **tokens, char *input, int *count, int start, int i)
{
	tokens[*count] = (char *)malloc(i - start + 1);
	strncpy(tokens[*count], &input[start], i - start);
	tokens[*count][i - start] = '\0';
	(*count)++;
	return (tokens);
}

char	**handle_special_chars(char *input, char **tokens, int *i, int *count)
{
	if ((input[*i] == '>' && input[*i + 1] == '>')
		|| (input[*i] == '<' && input[*i + 1] == '<'))
	{
		tokens[*count] = (char *)malloc(3);
		tokens[*count][0] = input[*i];
		tokens[*count][1] = input[*i + 1];
		tokens[*count][2] = '\0';
		(*i)++;
		(*count)++;
	}
	else if (input[*i] == '>' || input[*i] == '<' || input[*i] == '|')
	{
		tokens[*count] = (char *)malloc(2);
		tokens[*count][0] = input[*i];
		tokens[*count][1] = '\0';
		(*count)++;
	}
	return (tokens);
}

char	**tokenize_input(char *input)
{
	char	**tokens = allocate_tokens(input);
	int 	i = 0, start = 0, count = 0;

	while (input[i])
	{
		if (!is_allowed_char(input[i]))
		{
			print_error("fastshell", NULL, "syntax error", error_syntax);
			return (NULL);
		}
		if (input[i] == '\'' || input[i] == '"')
			handle_quotes(input, &i, input[i]);
		if (input[i] == ' ' || input[i] == '\t' || input[i] == '|'
			|| input[i] == '>' || input[i] == '<')
		{
			if (i > start)
				tokens = store_token(tokens, input, &count, start, i);
			tokens = handle_special_chars(input, tokens, &i, &count);
			start = i + 1;
		}
		i++;
	}
	if (i > start)
		tokens = store_token(tokens, input, &count, start, i);
	tokens[count] = NULL;
	return (tokens);
}
