#include "minishell.h"

static int	init_env_list(t_arg *arg, char **envp)
{
	int		i;
	t_list	*new;

	arg->env_list = NULL;
	i = 0;
	while (envp[i])
	{
		new = ft_lstnew(ft_strdup(envp[i]));
		if (!new || !new->content)
			print_error(NULL, NULL, NULL, error_systemcall);
		ft_lstadd_back(&(arg->env_list), new);
		i++;
	}
	return (1);
}

int	env_list_to_envp(t_arg *arg)
{
	int		size;
	int		i;
	t_list	*node;

	i = 0;
	if (arg->envp)
		while (arg->envp[i])
			free(arg->envp[i++]);
	free(arg->envp);
	size = ft_lstsize(arg->env_list);
	arg->envp = (char **)malloc(sizeof(char *) * (size + 1));
	if (!arg->envp)
		print_error(NULL, NULL, NULL, error_systemcall);
	node = arg->env_list;
	i = 0;
	while (node)
	{
		arg->envp[i] = ft_strdup(node->content);
		if (!arg->envp[i])
			print_error(NULL, NULL, NULL, error_systemcall);
		node = node->next;
		i++;
	}
	arg->envp[i] = NULL;
	return (1);
}

// last_exit_code 어떻게 할지 생각해야 함 ($?)
// 기존 STDIN, STDOUT 저장해놨다 써야할듯?
// built_in 하나만 들어오면 dup해야되는데 기존으로 바꿔줘야 함
int	init_arg(t_arg *arg, char **envp)
{
	arg->envp = NULL;
	if (!init_env_list(arg, envp) || !env_list_to_envp(arg))
		return (0);
	arg->cmd_list = (t_list *)malloc(sizeof(t_list *));
	if (!arg->cmd_list)
		return (0);
	arg->exit_code = EXIT_SUCCESS;
	arg->last_exit_code = -1;
	// 빌트인 하나만 들어왔을때 리디렉션 어캐할지 생각해야 함
	return (1);
}
