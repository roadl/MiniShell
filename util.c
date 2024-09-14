#include "minishell.h"

int ft_setenv(const char *name, const char *value, int overwrite, char **envp)
{
	char	*origin;

	while (envp)
	{
		if (ft_strncmp(*envp, name, ft_strlen(name)) == 0)
			break;
		envp++;
	}
}
