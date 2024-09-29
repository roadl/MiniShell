#include "minishell.h"

void	set_signal(void)
// 현재 프로세스가 터미널에 표시되고있을때의 시그널 상태로 설정
{
	set_terminal_print_off(); // 터미널에 ^C, ^\등의 시그널표식을 출력하지않도록 설정
	signal(SIGINT, do_sigint); // ctrl+c 시그널 입력시 do_sigint 함수 실행
	signal(SIGQUIT, SIG_IGN); // ctrl+\ 시그널 입력시 시그널을 무시함
}

void	set_signal_fork(void)
// 현재 프로세스가 자식 프로세스를 생성하고 종료를 기다리는동안의 시그널 상태로 설정
{
	signal(SIGINT, SIG_IGN); // ctrl+c 시그널 입력시 시그널을 무시함
	signal(SIGQUIT, SIG_IGN); // ctrl+\ 시그널 입력시 시그널을 무시함
}

void	set_signal_origin(void)
// 현재 프로세스가 시그널 동작을 기본 설정대로 동작하도록 원상복귀시켜줌
{
	signal(SIGINT, SIG_DFL); // ctrl+c 시그널 입력시 동작을 기본 시그널 설정으로 되돌림
	signal(SIGQUIT, SIG_DFL); // ctrl+\ 시그널 입력시 동작을 기본 시그널 설정으로 되돌림
}

void	set_signal_heredoc(void)
// heredoc을 실행하는 프로세스의 시그널 동작을 설정해줌
{
	signal(SIGINT, do_sigint_heredoc); // ctrl+c 시그널 입력시 do_sigint_heredoc함수 실행
	signal(SIGQUIT, SIG_IGN); // ctrl+\ 시그널 입력시 시그널을 무시함
}

void	check_fork_signal(int statloc)
// cat, grep등을 시그널로 빠져나갈때 동작함
{
	if ((statloc & 255) != 0)
    // 시그널로인해 종료되었을때 statloc에 저장되는 시그널값을 확인
	{
		if ((statloc & 127) == 2)
        // statloc을 비트마스킹하여 종료시그널이 무엇인지 확인함, 2는 ctrl+c
			ft_putstr_fd("\n", STDERR_FILENO);
		else if ((statloc & 127) == 3)
			ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
	}
}
