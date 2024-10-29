#include "minishell.h"

int	process_single_quote(char *new, char *token, t_arg *arg)
{
	arg->index_old++;
	while (token[arg->index_old] && token[arg->index_old] != '\'')
		new[arg->index_new++] = token[arg->index_old++];
	if (token[arg->index_old] != '\'')
	{
		free(token);
		free(new);
		print_error("fastshell", NULL, "\'", error_syntax);
		return (1);
	}
	arg->index_old++;
	return (0);
}

int	process_double_quote(char *new, char *token, t_arg *arg)
{
	arg->index_old++;
	while (token[arg->index_old] && token[arg->index_old] != '"')
	{
		if (token[arg->index_old] == '$' && ft_strcmp(new, "<<") != 0)
		{
			if (process_dollar(&new, token, arg))
				return (1);
			continue ;
		}
		new[arg->index_new++] = token[arg->index_old++];
	}
	if (token[arg->index_old] != '"')
	{
		free(token);
		free(new);
		print_error("fastshell", NULL, "\"", error_syntax);
		return (1);
	}
	arg->index_old++;
	return (0);
}

int	process_dollar(char **new, char *token, t_arg *arg)
{
	char	*env_value;

	env_value = replace_env_variable(token, arg);
	if (!env_value)
		return (1);
	arg->index_new += ft_strlen(env_value);
	*new = ft_strjoin_with_free(*new, env_value);
	return (0);
}
