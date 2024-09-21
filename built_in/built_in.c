#include "minishell.h"

// built_in 함수 전체 함수 하나로 처리해야할듯
// 리턴값 보고 에러시 exit? 어떻게 해야 하지

static char	*trans_path(char *path)
{
	char	*temp;
	int		i;

	if (path[ft_strlen(path)] == '/')
		temp = ft_strdup(path);
	else
		temp = ft_strjoin(path, "/");
	if (!temp)
		handle_systemcall_error();
}

// cd에서 들어온 첫번째 문자열 받아서 이동할 최종 path 리턴
// ~, -, ., .. 처리해야 함
// ., ..은 ./, ../일때 앞의 경로를 적당히 따서 바꿔주면 될듯
// ~, - 처리 한 다음에 ., .. 처리하는 함수 호출, 그 다음에 진행
static char	*get_path(char *find, t_arg *arg)
{
	char	*path;
	t_list	*node;

	if (!find || ft_strncmp(find, "~", 2) == 0)
	{
		node = find_env("HOME", arg);
		if (!node)
		{
			print_error("cd", NULL, "HOME not set", error_built_in);
			return (0);
		}
		return (ft_substr(node->content, 5, ft_strlen(node->content)));
	}
	else if (ft_strncmp(find, "-", 2) == 0)
	{
		node = find_env("OLDPWD", arg);
		if (!node)
		{
			print_error("cd", NULL, "OLDPWD not set", error_built_in);
			return (0);
		}
		return (ft_substr(node->content, 7, ft_strlen(node->content)));
	}
	return (ft_strdup(find));
}

// option -n 들어올 수 있음
// $?에 대한 처리 어떻게 할지 생각해야 함
int	ft_echo(char **cmd, t_arg *arg, int option)
{
	cmd++;
	while (*cmd)
	{
		printf("%s", *cmd);
		cmd++;
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
int	ft_cd(char **cmd, t_arg *arg)
{
	char	*cur;
	char	*path;
	t_list	*node;

	path = get_path(cmd[1], arg);
	if (!path)
		handle_systemcall_error();
	cur = getcwd(NULL, 0);
	if (!cur)
		handle_systemcall_error();
	if (chdir(path) == -1)
		return (print_error("cd", path, \
			"No such file or directory", error_built_in));
	free(path);
	update_env(ft_strjoin("OLDPWD=", cur), arg);
	cur = getcwd(NULL, 0);
	if (!cur)
		handle_systemcall_error();
	update_env(ft_strjoin("PWD=", cur), arg);
	return (0);
}

// 인자 없음 0
// 인자 있음 % 256
// 숫자 아니면 에러메세지
// 인자 2개 이상이면 에러메세지
// single command일때 exit 출력 후 종료
// 음수일땐 256을 뺀 값
// 숫자가 long long 넘어가면 에러메세지
int	ft_exit(char **cmd, t_arg *arg)
{
	int	exit_code;

	if (!cmd[1])
		exit(EXIT_SUCCESS);
	exit_code = ft_atoi(cmd[1]) % 256;
	if (exit_code < 0)
		exit_code = exit_code + 256;
	exit(exit_code);
}
