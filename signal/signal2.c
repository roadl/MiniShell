#include "minishell.h"

void	do_sigint_heredoc(int signum)
// heredoc에서 crtl+c 시그널 입력시 줄바꿈을 출력하고 1을 반환하며 종료됨
{
	(void)signum;
	ft_putstr_fd("\n", STDERR_FILENO); // 줄바꿈을 출력
	exit (1); // 1 을 반환하며 종료
}

void	set_terminal_print_off(void)
// 터미널에 ^C, ^\등의 시그널표식을 출력하지않도록 설정
{
	struct termios	term; // 터미널 설정이 담겨있는 termios 구조체

	tcgetattr(1, &term); // 현재 터미널의 설정을 term에 가져옴
	term.c_lflag &= ~(ECHOCTL); // 시그널표식 출력이 true 라면 false로 변경
	tcsetattr(1, 0, &term); // 변경한 term 설정을 현재 터미널에 적용
}

void	set_terminal_print_on(void)
// 터미널에 ^C, ^\등의 시그널표식을 출력하도록 설정
{
	struct termios	term; // 터미널 설정이 담겨있는 termios 구조체

	tcgetattr(1, &term); // 현재 터미널의 설정을 term에 가져옴
	term.c_lflag |= (ECHOCTL); // 시그널표식 출력이 false 라면 true로 변경
	tcsetattr(1, 0, &term);  // 변경한 term 설정을 현재 터미널에 적용
}

void	do_sigint(int signum)
// 기본 minishell 상태에서 ctrl+c입력을 받았을때 실행되는 함수
{
	(void)signum;
	ft_putstr_fd("\n", STDERR_FILENO); // 줄바꿈 출력
	rl_on_new_line(); // 커서가 다음줄로 옮겨간것을 readline에 적용
	rl_replace_line("", 0); // 현재 버퍼를 비워줌
	rl_redisplay(); // readline 메시지를 다시 출력
	// change_exit_status(1);
    // 저장중이던 최근 반환값을 1로 수정해줌 (ctrl+c 시그널을 작동할때 1의 반환값을 가져야함)
}

void	do_sigterm(void)
// ctrl+d를 눌렀을때 작동
{
	ft_putstr_fd("\033[1A", STDERR_FILENO); // 현재 커서의 위치를 한칸 위로 올려줌 
	ft_putstr_fd("\033[11C", STDERR_FILENO); // 현재 커서의 위치를 12번째칸으로 이동
	ft_putstr_fd("exit\n", STDERR_FILENO); // exit를 출력
}
