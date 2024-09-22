#include "minishell.h"

int	init_env_list(t_list **lst, char **envp)
{
	int		i;
	t_list	*new;

	i = 0;
	while (envp[i])
	{
		new = ft_lstnew(ft_strdup(envp[i]));
		if (!new || !new->content)
			handle_systemcall_error();
		ft_lstadd_back(lst, new);
		i++;
	}
	return (1);
}

char	**env_list_to_envp(t_list *lst, char **envp)
{
	int		size;
	int		i;
	t_list	*node;

	i = 0;
	if (envp)
		while (envp[i])
			free(envp[i++]);
	free(envp);
	size = ft_lstsize(lst);
	envp = (char **)malloc(sizeof(char *) * (size + 1));
	if (!envp)
		handle_systemcall_error();
	node = lst;
	i = 0;
	while (node)
	{
		envp[i] = ft_strdup(node->content);
		if (!envp[i])
			handle_systemcall_error();
		node = node->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

// last_exit_code 어떻게 할지 생각해야 함 ($?)
// 기존 STDIN, STDOUT 저장해놨다 써야할듯?
// built_in 하나만 들어오면 dup해야되는데 기존으로 바꿔줘야 함
int	init_arg(t_arg *arg, char **envp)
{
	arg->envp = NULL;
	arg->env_list = NULL;
	init_env_list(&arg->env_list, envp);
	arg->envp = env_list_to_envp(arg->env_list, arg->envp);
	arg->cmd_list = NULL;
	arg->last_exit_code = EXIT_SUCCESS;
	arg->origin_stdin = dup(STDIN_FILENO);
	arg->origin_stdout = dup(STDOUT_FILENO);
	return (1);
}
