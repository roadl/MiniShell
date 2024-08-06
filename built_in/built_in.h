#ifndef BUILT_IN_H
# define BUILT_IN_H

typedef enum e_built
{
	ERR = -1,
	ECHO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT
}	t_built;

int	ft_echo(char *cmd, int option, char **arg, char **envp);
int	ft_cd(char **cmd, char **envp);
int	ft_pwd(char **cmd, char **envp);
int	ft_export(char **cmd, char **envp);
int	ft_unset(char **cmd, char **envp);
int	ft_env(char **cmd, char **envp);
int	ft_exit(char **cmd, char **envp);

#endif
