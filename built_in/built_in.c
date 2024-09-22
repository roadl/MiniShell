#include "minishell.h"

// cd에서 들어온 첫번째 문자열 받아서 이동할 최종 path 리턴
// ~, -, 없을때 처리해야 함
static char	*get_path(char *find, t_list *env_list)
{
	t_list	*node;
	char	*path;

	node = NULL;
	if (!find || ft_strncmp(find, "~", 2) == 0)
		node = find_env("HOME", env_list);
	else
		node = find_env("OLDPWD", env_list);
	if (!node || !ft_strchr(node->content, '='))
	{
		if (!find || ft_strncmp(find, "~", 2) == 0)
			print_error("cd", NULL, "HOME not set", error_built_in);
		else if (ft_strncmp(find, "-", 2) == 0)
			print_error("cd", NULL, "OLDPWD not set", error_built_in);
		return (0);
	}
	if (!find || ft_strncmp(find, "~", 2) == 0)
		path = ft_substr(node->content, 5, ft_strlen(node->content));
	else
		path = ft_substr(node->content, 7, ft_strlen(node->content));
	if (!path)
		handle_systemcall_error();
	return (path);
}

static void	update_pwd(const char *key, t_list **env_list, char ***envp)
{
	char	*cur;
	char	*temp;

	cur = getcwd(NULL, 0);
	if (!cur)
		handle_systemcall_error();
	temp = ft_strjoin(key, cur);
	if (!temp)
		handle_systemcall_error();
	update_env(temp, env_list, envp);
	free(cur);
	free(temp);
}

// option -n 들어올 수 있음
// $?에 대한 처리 어떻게 할지 생각해야 함
int	ft_echo(t_cmd *cmd, int option)
{
	int	i;

	i = 1;
	while (cmd->argv[i])
	{	
		if (i != 1)
			printf(" ");
		printf("%s", cmd->argv[i++]);
	}
	if (option)
		printf("\n");
	return (0);
}

// 상대, 절대 경로
// 경로 오면 터짐 버근가
int	ft_cd(t_cmd *cmd, t_list **env_list, char ***envp)
{
	char	*path;

	if (!cmd->argv[1] || ft_strncmp(cmd->argv[1], "-", 2) == 0 || \
		ft_strncmp(cmd->argv[1], "~", 2) == 0)
		path = get_path(cmd->argv[1], *env_list);
	else
	{
		path = ft_strdup(cmd->argv[1]);
		if (!path)
			handle_systemcall_error();
	}
	if (!path)
		return (EXIT_FAILURE);
	update_pwd("OLDPWD=", env_list, envp);
	if (chdir(path) == -1)
		return (print_error("cd", path, \
			"No such file or directory", error_built_in));
	update_pwd("PWD=", env_list, envp);
	free(path);
	return (EXIT_SUCCESS);
}

// 인자 없음 0
// 인자 있음 % 256
// 숫자 아니면 에러메세지
// 인자 2개 이상이면 에러메세지
// single command일때 exit 출력 후 종료
// 음수일땐 256을 뺀 값
// 숫자가 long long 넘어가면 에러메세지
int	ft_exit(t_cmd *cmd)
{
	int	exit_code;

	if (!cmd->argv[1])
		exit(EXIT_SUCCESS);
	exit_code = ft_atoi(cmd->argv[1]) % 256;
	if (exit_code < 0)
		exit_code = exit_code + 256;
	exit(exit_code);
}
