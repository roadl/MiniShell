/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbel <jeongbel@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 16:12:05 by jeongbel          #+#    #+#             */
/*   Updated: 2024/10/31 16:12:06 by jeongbel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <errno.h>
# include <string.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <termios.h>
# include <sys/wait.h>
# include "libft/libft.h"

typedef struct s_arg
{
	int		last_exit_code;
	int		origin_stdin;
	int		origin_stdout;
	char	**envp;
	t_list	*env_list;
	t_list	*cmd_list;
	int		index_old;
	int		index_new;
}	t_arg;

typedef struct s_cmd
{
	char	*cmd;
	char	**argv;
	int		read_fd;
	int		write_fd;
	int		here_doc;
	int		is_child;
	t_list	*redi_list;
}	t_cmd;

typedef struct s_redi
{
	char	*redi;
	char	*file;
}	t_redi;

typedef struct s_state
{
	char	**tokens;
	int		count;
	int		i;
	int		start;
}	t_state;

// minishell.c
void	set_signal_status(void);

// parse.c
t_list	*allocate_cmds(int cmd_count);
void	store_redirection(t_list **redi_list, char **tokens, int *token_index);
void	tokens_to_cmd(char **tokens, t_cmd *cmd);
t_list	*parsing(char *input, int *cmd_count);

// tokenize.c
char	**allocate_tokens(char *input);
void	handle_quotes(char *input, int *i, char quote);
void	store_token(char *input, t_state *state);
void	handle_special_chars(char *input, t_state *state);
char	**tokenize_input(char *input);

// quote.c
char	*replace_env_variable(char *token, t_arg *arg);
char	*change_quotes(char *token, t_arg *arg, char *redi);
void	process_quotes(t_arg *arg);

// quote_util.c
int		process_single_quote(char *new, char *token, t_arg *arg);
int		process_double_quote(char *new, char *token, t_arg *arg, char *redi);
int		process_dollar(char **new, char *token, t_arg *arg);
void	init_quote_change(t_arg *arg, int *i, char *new, size_t len);

// cmd.c
void	make_cmd(t_arg *arg);

// parse_util.c
int		is_allowed_char(char c);
int		count_pipe(char **input);
int		is_cmd_empty(t_cmd *cmd);
int		is_redi_error(t_cmd *cmd);
t_state	*init_token_state(void);
char	**return_tokens(t_state *state);
char	**rm_redi_from_tokens(char **tokens);

// parse_error.c
int		check_cmd_error(t_list *cmd_list, int cmd_count);
int		check_redi_error(t_list *cmd_list);
int		is_input_error(char *input);

// built_in.c
int		ft_echo(t_cmd *cmd);
int		ft_cd(t_cmd *cmd, t_list **env_list, char ***envp);
int		ft_pwd(t_cmd *cmd);
int		ft_export(t_cmd *cmd, t_list **env_list, char ***envp);
int		ft_unset(t_cmd *cmd, t_list **env_list, char ***envp);
int		ft_env(t_cmd *cmd, char ***envp);
int		ft_exit(t_cmd *cmd);

void	export_no_option(t_list *env_list);
int		check_cd_path(char *path);
int		check_exit_numeric(char *exit_code);
int		check_echo_option(char *cmd);
void	update_pwd(const char *key, t_list **env_list, char ***envp);
int		check_export_arg(t_cmd *cmd, t_list *env_list, int i);

// env.c
typedef enum e_key_type
{
	EXPORT,
	UNSET
}	t_key_type;

int		update_env(char *str, t_list **env_list, char ***envp);
int		unset_env(char *key, t_list **env_list, char ***envp);
char	*get_shlvl(t_arg *arg);
t_list	*find_env(const char *key, t_list *env_list);

char	*get_env_key(t_list *node);
char	*get_env_value(t_list *node);

// error.c
typedef enum e_error_type
{
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
int		exec_built_in(t_cmd *cmd, t_arg *arg, t_list **env_list, char ***envp);
int		run_child_process(t_arg *arg, int *fd, t_list *node);
int		exec_cmds(t_arg *arg);
int		wait_childs(t_arg *arg, int f_pid, int l_pid);
void	dup_fd(t_cmd *cmd);
void	set_fd(t_arg *arg, t_cmd *cmd, t_list *node, int fd[2]);
void	check_cmd(t_cmd *cmd);

// redi.c
int		handle_redi(t_cmd *cmd);
void	handle_here_doc(t_cmd *cmd);

// init.c
int		init_arg(t_arg *arg, char **envp);
int		init_env_list(t_list **lst, char **envp);
char	**env_list_to_envp(t_list *lst, char **envp);

// debug.c
void	print_strs(char **strs);
void	print_cmd_list(t_list *lst);
void	print_redi_list(t_list *lst);
void	print_envp(char **envp);

// util.c
int		is_built_in(char *cmd);
int		is_only_built_in(t_arg *arg);
void	free_strs(void *strs);
void	free_cmd(void *cmd);
void	free_all(t_arg *arg);
int		ft_strcmp(const char *s1, const char *s2);
t_cmd	*index_cmd(t_list *lst, int index);
char	*ft_strjoin_with_free(char *s1, char *s2);
char	*ft_strncpy(char *dest, char *src, unsigned int n);
size_t	ft_strslen(char **strs);

// signal.c
void	set_signal(void);
void	set_signal_fork(void);
void	set_signal_origin(void);
void	set_signal_heredoc(void);
void	do_sigint_heredoc(int signum);
void	set_terminal_print_off(void);
void	set_terminal_print_on(void);
void	do_sigint(int signum);
void	do_sigterm(void);
void	check_fork_signal(int statloc);

#endif