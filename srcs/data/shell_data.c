/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoshin <yoshin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 19:06:19 by yoshin            #+#    #+#             */
/*   Updated: 2025/08/15 20:58:31 by yoshin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

#include "def.h"
#include "utils.h"
#include "hashmap.h"
#include "shell_data.h"

/**
 * @brief 전역적으로 접근 가능한 shell_data 구조체를 반환
 *
 * 프로그램 전체에서 공유되는 t_shell_data 인스턴스를 static으로 유지하며,
 * 해당 구조체의 주소를 반환한다.
 *
 * @return t_shell_data* 전역 shell_data 구조체의 포인터
 *
 * @note
 * - static 변수로 선언되어 한 번 생성 후 프로그램 종료 시까지 유지된다.
 * - 외부에서 직접 전역 변수를 선언하지 않고 안전하게 공유할 수 있는 방식이다.
 */
t_shell_data	*get_shell_data(void)
{
	static t_shell_data	shell_data;

	return (&shell_data);
}

/**
 * @brief shell_data 구조체 초기화
 *
 * 환경 변수(envp)를 해시맵에 저장하고, 터미널 속성 백업 및
 * 표준 입출력 FD 백업, 초기 상태 플래그를 설정한다.
 *
 * @param envp 프로그램 실행 시 전달받은 환경 변수 배열 (NULL 종료)
 * @return t_result COMPLETED(성공)
 *
 * @note
 * - 각 환경 변수는 extract_key()와 extract_value()로 분리 후 put_key_value()로 저장
 * - termios_backup에는 현재 터미널 속성을 저장
 * - last_status는 EXIT_SUCCESS(0)로 초기화
 * - in_pipe 플래그는 FALSE로 초기화
 * - stdin_fd, stdout_fd에는 표준 입출력의 FD를 dup2로 복사하여 저장
 */
t_result	init_shell_data(char *envp[])
{
	t_hash_map	*map;
	char		*key;
	char		*value;

	map = &((get_shell_data())->envp_map);
	while (*envp != NULL)
	{
		key = extract_key(*envp);
		value = extract_value(*envp);
		put_key_value(map, key, value);
		free(key);
		free(value);
		envp++;
	}
	(get_shell_data())->last_status = EXIT_SUCCESS;
	(get_shell_data())->in_pipe = FALSE;
	dup2(STDIN_FILENO, (get_shell_data())->stdin_fd);
	dup2(STDOUT_FILENO, (get_shell_data())->stdout_fd);
	return (COMPLETED);
}

/**
 * @brief shell_data 내부 자원 해제
 *
 * shell_data 구조체에 저장된 환경 변수 해시맵을 초기화하여
 * 메모리 누수를 방지한다.
 */
void	clear_shell_data(void)
{
	clear_hashmap(&((get_shell_data())->envp_map));
}

/**
 * @brief 현재 터미널 설정을 저장한다.
 *
 * STDIN의 termios 속성을 읽어 get_shell_data()->termios_backup에 저장한다.
 *
 * @note
 * - 이후 restore_terminal_settings()에서 복원할 수 있다.
 */
void	save_terminal_settings(void)
{
	tcgetattr(STDIN_FILENO, &((get_shell_data())->termios_backup));
}

/**
 * @brief 저장해둔 터미널 설정을 복원한다.
 *
 * get_shell_data()->termios_backup에 저장된 termios 값을
 * 즉시(TCSANOW) STDIN에 적용한다.
 */
void	restore_terminal_settings(void)
{
	tcsetattr(STDIN_FILENO, TCSANOW,
		&(get_shell_data())->termios_backup);
}
