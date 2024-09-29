#include "minishell.h"

void	set_signal(void)
// 현재 프로세스가 터미널에 표시되고있을때의 시그널 상태로 설정
{
	set_terminal_print_off();
	signal(SIGINT, do_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	set_signal_fork(void)
// 현재 프로세스가 자식 프로세스를 생성하고 종료를 기다리는동안의 시그널 상태로 설정
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	set_signal_origin(void)
// 현재 프로세스가 시그널 동작을 기본 설정대로 동작하도록 원상복귀시켜줌
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	set_signal_heredoc(void)
// heredoc을 실행하는 프로세스의 시그널 동작을 설정해줌
{
	signal(SIGINT, do_sigint_heredoc);
	signal(SIGQUIT, SIG_IGN);
}

void	check_fork_signal(int statloc)
// cat, grep등을 시그널로 빠져나갈때 동작함
{
	if ((statloc & 255) != 0)
	{
		if ((statloc & 127) == 2)
			ft_putstr_fd("\n", STDERR_FILENO);
		else if ((statloc & 127) == 3)
			ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
	}
}
