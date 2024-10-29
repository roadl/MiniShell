#include "minishell.h"

static int	print_export(char **envp)
{
	char	*chr;
	char	*key;
	int		i;

	i = -1;
	while (envp[++i])
	{
		if (envp[i][0] == '\0')
			continue ;
		chr = ft_strchr(envp[i], '=');
		printf("declare -x ");
		if (chr)
		{
			key = ft_substr(envp[i], 0, chr - envp[i]);
			if (!key)
				handle_systemcall_error();
			printf("%s=", key);
			printf("\"%s\"\n", chr + 1);
			free(key);
		}
		else
			printf("%s\n", envp[i]);
	}
	return (0);
}

static void	sort_envp(char **envp) {
    int i, j;
    int count = 0;

    // envp 배열의 크기(요소 개수) 계산
    while (envp[count] != NULL) {
        count++;
    }

    // Bubble Sort로 문자열 배열 정렬
    for (i = 0; i < count - 1; i++) {
        for (j = 0; j < count - i - 1; j++) {
            // 현재 문자열과 다음 문자열을 비교
            if (ft_strncmp(envp[j], envp[j + 1], ft_strlen(envp[j])) > 0) {
                // 두 문자열 포인터 교환
                char *temp = envp[j];
                envp[j] = envp[j + 1];
                envp[j + 1] = temp;
            }
        }
    }
}

void	export_no_option(t_list *env_list)
{
	char	**envp;
	int		i;

	envp = NULL;
	envp = env_list_to_envp(env_list, envp);
	sort_envp(envp);
	print_export(envp);
	i = 0;
	if (envp)
		while (envp[i])
			free(envp[i++]);
	free(envp);
}

int	check_export_arg(t_cmd *cmd, t_list *env_list, int i)
{
	t_list	*node;

	if (i == 1 && cmd->argv[i][0] == '-')
	{
		print_error("export", cmd->argv[i], \
			"invalid option", invalid_option);
		return (2);
	}
	node = find_env(cmd->argv[i], env_list);
	if (node && ft_strchr(node->content, '=') != 0 \
		&& ft_strchr(cmd->cmd, '=') == 0)
		return (1);
	return (0);
}
