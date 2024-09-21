#include "minishell.h"

// cd에서 들어온 첫번째 문자열 받아서 이동할 최종 path 리턴
// ~, -, ., .. 처리해야 함
// ., ..은 ./, ../일때 앞의 경로를 적당히 따서 바꿔주면 될듯
// ~, - 처리 한 다음에 ., .. 처리하는 함수 호출, 그 다음에 진행
static char	*get_path(char *find, t_cmd *cmd, t_list *env_list)
{
	t_list	*node;
	char	*path;

	if (!find || ft_strncmp(find, "~", 2) == 0)
	{
		node = find_env("HOME", env_list);
		if (!node || !ft_strchr(node->content, '='))
		{
			print_error("cd", NULL, "HOME not set", error_built_in);
			return (0);
		}
		path = ft_substr(node->content, 5, ft_strlen(node->content));
	}
	else if (ft_strncmp(find, "-", 2) == 0)
	{
		node = find_env("OLDPWD", env_list);
		if (!node || !ft_strchr(node->content, '='))
		{
			print_error("cd", NULL, "OLDPWD not set", error_built_in);
			return (0);
		}
		path = ft_substr(node->content, 7, ft_strlen(node->content));
	}
	path = ft_strdup(find);
	if (!path)
		handle_systemcall_error();
	return (path);
}

// option -n 들어올 수 있음
// $?에 대한 처리 어떻게 할지 생각해야 함
int	ft_echo(t_cmd *cmd, int option)
{
	cmd->argv++;
	while (*(cmd->argv))
	{
		printf("%s", *(cmd->argv));
		cmd->argv++;
	}
	if (option)
		printf("\n");
	return (0);
}

// 상대, 절대 경로
// $HOME은 파싱에서 바꿔서 전달해줄거임
// OLDPWD 설정해야 함
// - or ~면 getenv로 가져와서 path로 chdir
// 아니면 그냥 들어온거로 chdir
// 바꾸면 이전거 OLDPWD, 다음거 PWD로 보내야 함
// 구조 바꿔야 함 함수 하나 만들어서 path 받아오고
// pwd를 OLDPWD로, chdir하고 PWD 업데이트 하고 free(path)하는 식으로
int	ft_cd(t_cmd *cmd, t_list **env_list, char ***envp)
{
	char	*cur;
	char	*path;
	t_list	*node;

	path = get_path(cmd->argv[1], cmd, *env_list);
	if (!path)
		return (EXIT_FAILURE);
	cur = getcwd(NULL, 0);
	if (!cur)
		handle_systemcall_error();
	if (chdir(path) == -1)
		return (print_error("cd", path, \
			"No such file or directory", error_built_in));
	free(path);
	update_env(ft_strjoin("OLDPWD=", cur), env_list, envp);
	cur = getcwd(NULL, 0);
	if (!cur)
		handle_systemcall_error();
	update_env(ft_strjoin("PWD=", cur), env_list, envp);
	return (0);
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
