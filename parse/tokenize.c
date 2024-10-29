#include "minishell.h"

char	**allocate_tokens(char *input)
{
	char	**tokens;

	tokens = (char **)malloc(sizeof(char *) * (ft_strlen(input) + 1));
	if (!tokens)
		handle_systemcall_error();
	return (tokens);
}

void	handle_quotes(char *input, int *i, char quote)
{
	(*i)++;
	while (input[*i] && input[*i] != quote)
		(*i)++;
}

void	store_token(char *input, t_token_state *state)
{
	state->tokens[state->count] = (char *)malloc(state->i - state->start + 1);
	ft_strncpy(state->tokens[state->count],
		&input[state->start], state->i - state->start);
	state->tokens[state->count][state->i - state->start] = '\0';
	(state->count)++;
}

void	handle_special_chars(char *input, t_token_state *state)
{
	if ((input[state->i] == '>' && input[state->i + 1] == '>')
		|| (input[state->i] == '<' && input[state->i + 1] == '<'))
	{
		state->tokens[state->count] = (char *)malloc(3);
		state->tokens[state->count][0] = input[state->i];
		state->tokens[state->count][1] = input[state->i + 1];
		state->tokens[state->count][2] = '\0';
		(state->i)++;
		(state->count)++;
	}
	else if (input[state->i] == '>' || input[state->i] == '<'
		|| input[state->i] == '|')
	{
		state->tokens[state->count] = (char *)malloc(2);
		state->tokens[state->count][0] = input[state->i];
		state->tokens[state->count][1] = '\0';
		(state->count)++;
	}
}

char	**tokenize_input(char *input)
{
	t_token_state	*state;

	if (!input || is_input_error(input))
		return (NULL);
	state = init_token_state();
	state->tokens = allocate_tokens(input);
	while (input[state->i])
	{
		if (input[state->i] == '\'' || input[state->i] == '"')
			handle_quotes(input, &state->i, input[state->i]);
		if (input[state->i] == ' ' || input[state->i] == '\t'
			|| input[state->i] == '|' || input[state->i] == '>'
			|| input[state->i] == '<')
		{
			if (state->i > state->start)
				store_token(input, state);
			handle_special_chars(input, state);
			state->start = state->i + 1;
		}
		state->i++;
	}
	if (state->i > state->start)
		store_token(input, state);
	state->tokens[state->count] = NULL;
	return (state->tokens);
}
