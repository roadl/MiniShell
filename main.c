#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int	main(int argc, char **argv, char **envp)
{
	char	*line;

	if (argc != 1)
		exit(EXIT_FAILURE);
	while (1)
	{
		// signal 설정
		line = readline("fastshell$ ");
		if (line == NULL)
			;
		if (*line == '\0')
			continue ;
		// set_signal_ignore?
		// 파싱
		add_history(line);
		free(line);
		// 실행
		// free, unlink
	}
}
