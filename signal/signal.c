#include "minishell.h"

void	set_signal(void)
{
	set_terminal_print_off(); // 터미널에 ^C, ^\등의 시그널표식을 출력하지않도록 설정
	signal(SIGINT, do_sigint); // ctrl+c 시그널 입력시 do_sigint 함수 실행
	signal(SIGQUIT, SIG_IGN); // ctrl+\ 시그널 입력시 시그널을 무시함
}
