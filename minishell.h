#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <errno.h>
# include <string.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include "libft/libft.h"

typedef struct s_arg {
	int		last_exit_code;
	int		origin_stdin;
	int		origin_stdout;
	char	**envp;
	t_list	*env_list;
	t_list	*cmd_list;
}	t_arg;

typedef struct s_cmd {
	char	*cmd;
	char	**argv;
	int		read_fd;
	int		write_fd;
	t_list	*redi_list;
}	t_cmd;

typedef struct s_redi {
	char	*redi;
	char	*file;
}	t_redi;


// built_in.c
int		ft_echo(t_cmd *cmd, int option);
int		ft_cd(t_cmd *cmd, t_list **env_list, char ***envp);
int		ft_pwd(t_cmd *cmd);
int		ft_export(t_cmd *cmd, t_list **env_list, char ***envp);
int		ft_unset(t_cmd *cmd, t_list **env_list, char ***envp);
int		ft_env(t_cmd *cmd, char ***envp);
int		ft_exit(t_cmd *cmd);

// env.c
int		update_env(char *str, t_list **env_list, char ***envp);
int		unset_env(char *key, t_list **env_list, char ***envp);
t_list	*find_env(const char *key, t_list *env_list);

char	*get_env_key(t_list *node);
char	*get_env_value(t_list *node);

// error.c
typedef enum e_error_type {
	error_systemcall = -127,
	error_syntax,
	error_access,
	error_redirection,
	error_ambiguous_redirection,
	error_file,
	error_max_heredoc,
	error_built_in = -1,
	invalid_identifier,
	invalid_option
}	t_error_type;

void	handle_systemcall_error(void);
int		print_error(char *cmd, char *arg, char *msg, t_error_type err_type);

// execute.c
int		exec_built_in(t_cmd *cmd, t_list **env_list, char ***envp);
int		run_child_process(t_arg *arg, int *fd, t_list *node);

// redi.c
void	handle_redi(t_cmd *cmd);

// init.c
int		init_arg(t_arg *arg, char **envp);
int		init_env_list(t_list **lst, char **envp);
char	**env_list_to_envp(t_list *lst, char **envp);

// debug.c
void	print_strs(char **strs);
void	print_cmd_list(t_list *lst);
void	print_envp(char **envp);

// util.c
int		is_built_in(char *cmd);
int		is_only_built_in(t_arg *arg);
void	free_strs(void *strs);
void	free_cmd(void *cmd);
void	free_all(t_arg *arg);

#endif