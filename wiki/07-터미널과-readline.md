# 07. 터미널과 readline

## 터미널이란

**터미널(terminal)** 은 원래 원격 컴퓨터에 접속하는 물리적 단말기(VT100 같은)였습니다. 현대에는 **터미널 에뮬레이터**(gnome-terminal, iTerm2, Windows Terminal 등)가 그 역할을 소프트웨어로 대체했고, 커널은 여전히 "터미널이 연결돼 있다"고 보고 입출력을 관리합니다.

유닉스 관점에서 터미널은 **특별한 파일**(`/dev/tty`, `/dev/pts/N`)입니다. 이 파일에 write 하면 화면에 글자가 나타나고, read 하면 키보드 입력을 받을 수 있습니다. fd 0/1/2 가 대화형 셸에서는 이 터미널 파일을 가리킵니다.

## 터미널 드라이버 — line discipline

커널의 **라인 디시플린(line discipline)** 은 키보드 입력을 가공해서 프로세스에게 전달합니다. 두 가지 모드:

### Canonical (정규) 모드 (기본)

- 한 줄 단위로 버퍼링 — Enter 를 쳐야 read(2) 가 반환.
- **라인 편집** 지원 — Backspace, Delete, Ctrl-W, Ctrl-U 등이 커널에서 처리돼 프로세스에는 최종 줄만 전달.
- **시그널 변환** — Ctrl-C → SIGINT, Ctrl-\\ → SIGQUIT, Ctrl-Z → SIGTSTP, Ctrl-D → EOF.
- **echo** — 입력한 글자를 자동으로 화면에도 출력.

### Raw 모드

- 글자 하나하나 즉시 프로세스에 전달.
- 시그널·echo·라인 편집 모두 비활성화(원하는 건 명시적으로 켬).
- 에디터(vim, emacs), 페이저(less), 셸의 readline 등이 **자체 편집 기능**을 구현하려고 raw 모드로 전환.

### 모드 전환: `termios`

```c
#include <termios.h>
struct termios	term;
tcgetattr(STDIN_FILENO, &term);   /* 현재 설정 백업 */

/* raw 모드로 전환 */
term.c_lflag &= ~(ICANON | ECHO);
tcsetattr(STDIN_FILENO, TCSANOW, &term);

/* ... 작업 ... */

/* 원래대로 복원 */
tcsetattr(STDIN_FILENO, TCSANOW, &orig_term);
```

| 필드 | 의미 |
|------|------|
| `c_lflag` | 로컬 모드 (`ICANON`, `ECHO`, `ISIG` 등) |
| `c_iflag` | 입력 변환 (`ICRNL`, `IXON` 등) |
| `c_oflag` | 출력 변환 (`OPOST`, `ONLCR` 등) |
| `c_cflag` | 제어 모드 (보드레이트 등) |

### 왜 셸에서 termios 가 필요한가

1. **Ctrl-C 출력 방지** — 터미널은 Ctrl-C 를 누르면 `^C` 를 에코합니다. 셸은 이걸 때로 억제하고 싶을 수 있음(기본 설정으로 두면 readline 이 알아서 처리).
2. **상태 복원** — 사용자가 외부 프로그램(`vim`)을 실행하고 비정상 종료해서 터미널이 깨졌을 때, 셸 시작 시 `tcgetattr` 로 백업해뒀다가 각 명령 실행 후 `tcsetattr` 로 복원합니다.
3. **비대화형 모드 감지**: `isatty(fd)` 가 터미널인지 확인.

## `isatty`, `ttyname`

```c
int	isatty(int fd);          /* 1 if fd is a terminal, else 0 */
char	*ttyname(int fd);        /* 터미널 장치 이름 ("/dev/pts/3" 등) */
```

셸이 대화형인지 판단할 때:

```c
if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
    /* 대화형 — readline 사용 */;
else
    /* 파이프/리다이렉트된 stdin — get_next_line 사용 */;
```

입력 함수 안에서 이 분기를 먼저 하는 것이 표준 패턴.

## `ioctl(2)` — 장치 제어

```c
int	ioctl(int fd, unsigned long request, ...);
```

파일 디스크립터에 대해 **장치별 제어**를 합니다. 터미널에서 자주 쓰는 요청:

- `TIOCGWINSZ` — 터미널 창 크기 얻기.
- `TIOCSTI` — 입력 버퍼에 글자 삽입(위험).

minishell 에서는 거의 쓸 일이 없습니다. 허용 목록에 있지만 실제로는 termios 로 충분.

## termcap — 화면 제어 코드

터미널마다 커서 이동·색 변경 등을 하는 **이스케이프 시퀀스가 다릅니다**(VT100 vs xterm vs Linux console). 호환성을 위해 **termcap/terminfo** 데이터베이스에 터미널별 정보가 저장돼 있고, 프로그램은 이 DB를 조회해서 올바른 시퀀스를 얻습니다.

### 허용 함수

```c
#include <termcap.h>
int	tgetent(char *bp, const char *name);     /* 터미널 DB 로드 */
int	tgetflag(char *id);                       /* 플래그 얻기 */
int	tgetnum(char *id);                        /* 숫자 얻기 */
char	*tgetstr(char *id, char **area);           /* 문자열 시퀀스 얻기 */
char	*tgoto(const char *cap, int col, int row); /* 시퀀스에 좌표 치환 */
int	tputs(const char *str, int affcnt,
	      int (*putc)(int));                 /* 시퀀스 출력 */
```

### 언제 쓰나?

서브젝트 허용 목록에 있지만, 실제로 mandatory 파트에서는 **readline 이 내부에서 이걸 대신 처리**해주기 때문에 직접 쓸 일은 거의 없습니다. 복잡한 자체 편집기를 만들 때나 필요.

## GNU Readline

**GNU Readline** 은 CLI 프로그램에 **라인 편집·히스토리·자동완성** 기능을 쉽게 붙일 수 있게 해주는 라이브러리입니다. bash, gdb, psql 등이 모두 readline 을 씁니다.

### 설치

- Ubuntu/Debian: `sudo apt install libreadline-dev`
- macOS: `brew install readline` (경로는 `/opt/homebrew/opt/readline`)

컴파일·링크 시:

```sh
cc main.c -lreadline -I/opt/homebrew/opt/readline/include -L/opt/homebrew/opt/readline/lib
```

Makefile 에서 OS 감지(`uname -s`)로 readline 경로를 분기시키는 패턴이 흔합니다(Linux 에서는 기본 경로라 빈 값).

### 핵심 API

#### `readline(3)` — 프롬프트를 표시하고 한 줄 입력받기

```c
#include <readline/readline.h>
char	*readline(const char *prompt);
```

- `prompt` 를 터미널에 출력.
- 사용자가 Enter 를 누를 때까지 **편집 가능한 버퍼**에서 입력을 받음.
- 리턴값은 **malloc 으로 할당된 문자열**(개행 없음). 호출자가 `free()` 해야 함.
- **Ctrl-D 를 빈 줄에서 치면 `NULL` 리턴** — EOF.

```c
char *line = readline("minishell> ");
if (line == NULL)
{
    /* EOF — 셸 종료 */
}
/* 사용 후 */
free(line);
```

#### `add_history(3)` — 히스토리에 추가

```c
#include <readline/history.h>
void	add_history(const char *line);
```

이걸 부르면 이후 up arrow 로 해당 줄을 불러올 수 있습니다.

```c
char *line = readline("> ");
if (line && *line)        /* 빈 줄은 히스토리에 안 넣는 게 관례 */
    add_history(line);
```

#### `rl_clear_history(3)`

히스토리 전체 삭제. 종료 시 정리용.

#### 시그널 핸들러 안에서 쓰는 함수들

```c
void	rl_on_new_line(void);      /* "우리 이제 새 줄에 있음" 을 readline 에게 알림 */
int	rl_replace_line(const char *text, int clear_undo);  /* 현재 편집 중인 줄을 교체 */
void	rl_redisplay(void);        /* 프롬프트와 버퍼 재출력 */
```

SIGINT 핸들러에서 새 프롬프트를 깨끗하게 보이려면:

```c
void	sigint_handler(int signo)
{
    write(STDERR_FILENO, "\n", 1);  /* 새 줄로 */
    rl_on_new_line();                /* readline 에게 알림 */
    rl_replace_line("", 0);          /* 편집 중이던 내용 버림 */
    rl_redisplay();                  /* 프롬프트 재표시 */
}
```

(위 함수들은 엄밀히는 async-signal-safe 가 아니지만, readline 이 잘 동작하는 것을 전제로 bash 를 포함해 많은 셸이 이 패턴을 씀.)

### readline 의 메모리 누수

`readline()` 은 내부에 정적 할당된 버퍼를 많이 쓰는데, **프로그램 종료 시점에 해제되지 않습니다**. `valgrind` 는 이걸 "definitely lost" 로 보고합니다.

서브젝트는 이 누수를 **허용**합니다. 하지만 본인 코드의 누수는 허용 안 됨. 검사 시 readline 누수를 제외하려면 suppression 파일을 씁니다(자세한 건 [14. 메모리 관리](14-메모리-관리)).

### readline 과 시그널

readline 은 기본적으로 시그널을 "가로챕니다". `rl_catch_signals = 0` 으로 하면 완전히 수동 제어가 가능하지만 복잡해집니다.

권장 기본 전략:

1. 셸이 `sigaction(SIGINT, ...)` 으로 자체 핸들러 설치.
2. `readline()` 이 시그널로 중단되면 내부적으로 재시작해서 **새 프롬프트를 다시 그립니다**.
3. 핸들러에서 `rl_on_new_line / replace / redisplay` 호출해 화면을 정돈.

## 프롬프트 이스케이프 문자

프롬프트에 색을 넣으면 예쁘지만, **이스케이프 시퀀스의 폭을 readline 이 0으로 계산**하도록 알려줘야 합니다. 안 그러면 긴 줄 편집 시 커서가 어긋남.

```c
#define RED_START    "\001\033[31m\002"
#define COLOR_RESET  "\001\033[0m\002"
```

- `\001` (RL_PROMPT_START_IGNORE) 와 `\002` (RL_PROMPT_END_IGNORE) 사이는 readline 이 **길이 0으로 계산**.

색 있는 프롬프트를 쓰면서 이 가드 문자를 빠뜨리면, 긴 입력을 편집할 때 커서 위치가 어긋나는 증상이 생깁니다.

## 예제: 최소 readline 셸 루프

```c
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int main(void)
{
    char *line;

    while ((line = readline("mini> ")) != NULL)
    {
        if (*line)
            add_history(line);
        /* ... 여기서 tokenize / parse / exec ... */
        printf("got: %s\n", line);
        free(line);
    }
    return 0;
}
```

컴파일:

```sh
cc example.c -lreadline -o mini
./mini
```

## 자주 하는 실수

### 1. `readline` 리턴값을 `free` 하지 않기
`readline()` 이 돌려준 포인터는 malloc 된 것이므로 반드시 free. 루프마다 free 하지 않으면 누수.

### 2. Ctrl-D 를 EOF 로 인식하지 않기
`readline()` 이 NULL 리턴하면 반드시 루프 탈출. 안 그러면 끝없는 루프.

### 3. 비대화형 입력에서 `readline` 호출
`./minishell < script.sh` 같은 경우 `isatty(STDIN_FILENO)` 가 거짓. `readline` 을 그대로 쓰면 프롬프트가 `stdout`(=아직 터미널) 에 찍히고 읽기는 리다이렉트된 stdin 에서 함 — 의도와 안 맞음. `get_next_line` 같은 직접 read 로 분기.

### 4. SIGINT 핸들러에서 `printf` 호출
async-signal-safe 아님. `write` + readline 함수만.

### 5. `add_history` 를 빈 줄에도 호출
`""` 도 히스토리에 쌓이면 up arrow 가 이상해짐. 빈 줄은 스킵.

### 6. 터미널 상태 복원을 빠뜨림
자식 프로그램이 raw 모드로 바꿔놓고 비정상 종료하면 터미널이 이상해짐. `tcgetattr` 로 백업해두고, fork 후 waitpid 복귀 시 `tcsetattr` 로 복원.

## 관련 맨페이지 / 문서

```sh
man 3 readline
man 3 termios
man 3 tputs
```

- [GNU Readline 공식 문서](https://tiswww.case.edu/php/chet/readline/readline.html)

## 다음 읽을 페이지

- [08. 렉서 (토크나이저)](08-렉서-토크나이저)
