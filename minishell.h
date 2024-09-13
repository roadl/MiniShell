#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "libft/libft.h"

// built_in.c
int	ft_echo(char **arg, char **envp, int option);
int	ft_cd(char **cmd, char **envp);
int	ft_pwd(char **cmd, char **envp);
int	ft_export(char **cmd, char **envp);
int	ft_unset(char **cmd, char **envp);
int	ft_env(char **cmd, char **envp);
int	ft_exit(char **cmd, char **envp);

#endif