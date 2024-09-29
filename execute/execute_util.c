#include "minishell.h"

int	wait_childs(t_arg *arg, int f_pid, int l_pid)
{
	int	statloc;

	
	waitpid(f_pid, &statloc, 0);
	if ((statloc & 255) != 0)
	{
		if ((statloc & 127) == 2)
			ft_putstr_fd("\n", STDERR_FILENO);
		else if ((statloc & 127) == 3)
			ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
	}
	if (l_pid != 0)
		waitpid(l_pid, &statloc, 0);
	if (WIFEXITED(statloc))
		arg->last_exit_code = WEXITSTATUS(statloc);
	else
		arg->last_exit_code = 128 + WTERMSIG(statloc);
	wait(0);
	return (arg->last_exit_code);
}
