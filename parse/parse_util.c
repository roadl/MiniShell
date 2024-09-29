#include "minishell.h"

int	is_allowed_char(char c)
{
	if (ft_isalnum(c) || c == '_' || c == '<' || c == '>' 
		|| c == '|' || c == '"' || c == '\'' || c == '$'
			|| c == ' ' || c == '\t' || c == '=' || c == '~' || c == '?')
		return (1);
	return (0);
}