/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 22:54:51 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/05 23:17:01 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include <readline/readline.h>
// #include <signal.h>
// #include <stdlib.h>
// #include <termios.h>
// #include <unistd.h>
// 
// #include "libft.h"
// 
// #include "mini_signal.h"
// 
// void	set_signal(void)
// // 현재 프로세스가 터미널에 표시되고있을때의 시그널 상태로 설정
// {
// 	set_terminal_print_off(); // 터미널에 ^C, ^\등의 시그널표식을 출력하지않도록 설정
// 	signal(SIGINT, do_sigint); // ctrl+c 시그널 입력시 do_sigint 함수 실행
// 	signal(SIGQUIT, SIG_IGN); // ctrl+\ 시그널 입력시 시그널을 무시함
// }
// 
// void	set_signal_fork(void)
// // 현재 프로세스가 자식 프로세스를 생성하고 종료를 기다리는동안의 시그널 상태로 설정
// {
// 	signal(SIGINT, SIG_IGN); // ctrl+c 시그널 입력시 시그널을 무시함
// 	signal(SIGQUIT, SIG_IGN); // ctrl+\ 시그널 입력시 시그널을 무시함
// }
// 
// void	set_signal_origin(void)
// // 현재 프로세스가 시그널 동작을 기본 설정대로 동작하도록 원상복귀시켜줌
// {
// 	signal(SIGINT, SIG_DFL); // ctrl+c 시그널 입력시 동작을 기본 시그널 설정으로 되돌림
// 	signal(SIGQUIT, SIG_DFL); // ctrl+\ 시그널 입력시 동작을 기본 시그널 설정으로 되돌림
// }
// 
// void	set_signal_heredoc(void)
// // heredoc을 실행하는 프로세스의 시그널 동작을 설정해줌
// {
// 	signal(SIGINT, do_sigint_heredoc); // ctrl+c 시그널 입력시 do_sigint_heredoc함수 실행
// 	signal(SIGQUIT, SIG_IGN); // ctrl+\ 시그널 입력시 시그널을 무시함
// }
// 
// void	do_sigint_heredoc(int signum)
// // heredoc에서 crtl+c 시그널 입력시 줄바꿈을 출력하고 1을 반환하며 종료됨
// {
// 	(void)signum;
// 	ft_putstr_fd("\n", STDERR_FILENO); // 줄바꿈을 출력
// 	exit (1); // 1 을 반환하며 종료
// }
// 
// void	set_terminal_print_off(void)
// // 터미널에 ^C, ^\등의 시그널표식을 출력하지않도록 설정
// {
// 	struct termios	term; // 터미널 설정이 담겨있는 termios 구조체
// 
// 	tcgetattr(1, &term); // 현재 터미널의 설정을 term에 가져옴
// 	term.c_lflag &= ~(ECHOCTL); // 시그널표식 출력이 true 라면 false로 변경
// 	tcsetattr(1, 0, &term); // 변경한 term 설정을 현재 터미널에 적용
// }
// 
// void	set_terminal_print_on(void)
// // 터미널에 ^C, ^\등의 시그널표식을 출력하도록 설정
// {
// 	struct termios	term; // 터미널 설정이 담겨있는 termios 구조체
// 
// 	tcgetattr(1, &term); // 현재 터미널의 설정을 term에 가져옴
// 	term.c_lflag |= (ECHOCTL); // 시그널표식 출력이 false 라면 true로 변경
// 	tcsetattr(1, 0, &term);  // 변경한 term 설정을 현재 터미널에 적용
// }
// 
// void	do_sigint(int signum)
// // 기본 minishell 상태에서 ctrl+c입력을 받았을때 실행되는 함수
// {
// 	(void)signum;
// 	ft_putstr_fd("\n", STDERR_FILENO); // 줄바꿈 출력
// 	rl_on_new_line(); // 커서가 다음줄로 옮겨간것을 readline에 적용
// 	rl_replace_line("", 0); // 현재 버퍼를 비워줌
// 	rl_redisplay(); // readline 메시지를 다시 출력
// 	change_exit_status(1);
//     // 저장중이던 최근 반환값을 1로 수정해줌 (ctrl+c 시그널을 작동할때 1의 반환값을 가져야함)
// }
// 
// void	do_sigterm(void)
// // ctrl+d를 눌렀을때 작동
// {
// 	ft_putstr_fd("\033[1A", STDERR_FILENO); // 현재 커서의 위치를 한칸 위로 올려줌 
// 	ft_putstr_fd("\033[12C", STDERR_FILENO); // 현재 커서의 위치를 12번째칸으로 이동
// 	ft_putstr_fd("exit\n", STDERR_FILENO); // exit를 출력
// }
// 
// void	check_fork_signal(int statloc)
// // cat, grep등을 시그널로 빠져나갈때 동작함
// {
// 	if ((statloc & 255) != 0)
//     // 시그널로인해 종료되었을때 statloc에 저장되는 시그널값을 확인
// 	{
// 		if ((statloc & 127) == 2)
//         // statloc을 비트마스킹하여 종료시그널이 무엇인지 확인함, 2는 ctrl+c
// 			ft_putstr_fd("\n", STDERR_FILENO);
// 		else if ((statloc & 127) == 3)
//         /* statloc을 비트마스킹하여 종료시그널이 무엇인지 확인함, 3은 ctrl+\ */
// 			ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
// 	}
// }
