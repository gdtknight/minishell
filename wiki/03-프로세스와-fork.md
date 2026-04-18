# 03. 프로세스와 fork

셸이 하는 일의 핵심은 "다른 프로그램을 실행하는 것"이고, 그 구현은 전적으로 **프로세스 모델**에 의존합니다. 이 문서는 프로세스, `fork`, `execve`, `wait` 계열을 바닥부터 설명합니다.

## 프로세스란?

**프로세스(process)** 는 **실행 중인 프로그램의 인스턴스**입니다. 같은 `ls` 바이너리를 두 번 실행하면 프로세스는 두 개입니다.

각 프로세스는 다음을 **독점적으로** 소유합니다.

- **가상 주소 공간** — 코드·데이터·스택·힙을 담는 메모리.
- **프로세스 ID(PID)** — 커널이 부여하는 고유 정수.
- **파일 디스크립터 테이블** — 열린 파일·파이프·소켓 목록.
- **시그널 핸들러 테이블**.
- **환경 변수·현재 작업 디렉터리**.
- **자원 사용량**, **사용자/그룹 ID**, **부모 프로세스 ID(PPID)** 등.

```sh
ps -ef | head     # 현재 시스템의 프로세스 트리 일부
```

각 프로세스는 **정확히 하나의 부모**를 가지며(PID 1인 init 제외), 트리를 이룹니다. `fork()` 는 이 트리에 새 가지를 만드는 유일한 방법입니다(`pthread_create` 는 프로세스가 아니라 스레드).

## `fork(2)` — 프로세스 복제

```c
#include <unistd.h>
pid_t	fork(void);
```

`fork()` 는 현재 프로세스를 **거의 완벽하게 복제**합니다. 호출 후에는 "부모"와 "자식" 두 프로세스가 **각자 fork 직후 지점부터 실행을 계속**합니다.

### 반환값의 마법

`fork()` 는 한 번 호출되는데 **두 번 반환**됩니다.

| 반환값 | 누가 받는가 |
|--------|-------------|
| **양수 (자식의 PID)** | 부모 프로세스 |
| **0** | 자식 프로세스 |
| **-1** | 실패. 이때 `errno` 확인 |

이 규칙으로 한 코드 조각 안에서 부모/자식 분기를 구현합니다.

```c
pid_t pid = fork();
if (pid == -1)
{
    perror("fork");
    exit(1);
}
else if (pid == 0)
{
    /* 자식 */
    printf("child: my pid = %d, my parent = %d\n", getpid(), getppid());
    exit(0);
}
else
{
    /* 부모 */
    printf("parent: my pid = %d, child pid = %d\n", getpid(), pid);
    wait(NULL);
}
```

### fork 직후 복제되는 것과 그렇지 않은 것

**복제됨** (자식은 부모와 동일):

- 힙·스택·정적 영역의 **메모리 내용**(Copy-on-Write로 실제 복사는 쓸 때까지 지연).
- **파일 디스크립터 테이블** — 부모가 fd 3을 열어둔 상태면 자식도 fd 3으로 같은 파일을 가리킴.
- 환경 변수, cwd, umask, 시그널 핸들러 설정.

**복제되지 않음** (자식이 따로 가짐):

- **PID / PPID** — 자식은 새 PID를 받고, PPID는 부모의 PID.
- **자원 사용량 통계** — 자식은 0부터 시작.
- **pending 시그널과 파일 락** — 자식은 없음으로 초기화.

### Copy-on-Write (CoW)

`fork()` 가 전체 메모리를 복사하면 비싸겠지만, 현대 리눅스는 **페이지 단위로 공유**해두고 **쓰기 시점에만 복사**합니다. 그래서 `fork` 는 매우 저렴합니다.

## `exec` 계열 — 프로세스 이미지 교체

**`fork()` 는 같은 프로그램을 복제할 뿐**입니다. 다른 프로그램을 실행하려면 `exec` 를 써야 합니다.

`exec` 계열은 현재 프로세스의 **메모리 이미지를 통째로 새 프로그램으로 교체**합니다. PID는 유지되지만, 실행 코드·데이터·스택이 모두 바뀝니다. 성공하면 **원래 코드로 돌아오지 않습니다**(새 프로그램의 `main`부터 시작).

### 42가 허용하는 건 `execve` 하나

```c
#include <unistd.h>
int	execve(const char *pathname,
	       char *const argv[],
	       char *const envp[]);
```

| 인자 | 설명 |
|------|------|
| `pathname` | 실행 파일의 **절대·상대 경로**(PATH 검색을 해주지 않음!) |
| `argv` | 프로그램에 전달할 인자 배열. `argv[0]` 이 프로그램 이름, 마지막은 `NULL` |
| `envp` | 환경 변수 배열. `"KEY=VALUE"` 문자열들, 마지막은 `NULL` |

```c
char *argv[] = {"/bin/ls", "-l", "/tmp", NULL};
char *envp[] = {"PATH=/bin:/usr/bin", "HOME=/root", NULL};
execve("/bin/ls", argv, envp);
perror("execve");  /* execve가 실패해야만 여기 도달 */
exit(127);
```

### execve는 PATH를 뒤지지 않는다

bash에서 `ls` 를 치면 셸이 `PATH` 를 분할해 `/usr/bin/ls`, `/bin/ls` 순서로 찾아줍니다. `execve` 는 그걸 **하지 않습니다**. 셸이 직접:

1. `PATH` 를 `:` 로 분할.
2. 각 디렉터리에 대해 `디렉터리/명령` 경로를 만들어 `access(path, X_OK)` 확인.
3. 처음으로 존재하고 실행 가능한 것을 `execve` 에 넘김.

이 로직은 [13. PATH 검색과 실행](13-PATH-검색과-실행) 에서 자세히.

## fork + exec 패턴

셸에서 외부 명령을 실행하는 표준 시퀀스:

```c
pid_t pid = fork();
if (pid == 0)
{
    /* 자식: 새 프로그램으로 교체 */
    execve(path, argv, envp);
    perror("execve");
    exit(127);       /* execve 실패 시 반드시 exit */
}
else if (pid > 0)
{
    /* 부모: 자식이 끝나길 대기 */
    int status;
    waitpid(pid, &status, 0);
}
```

**반드시 자식에서 `exit()` 로 끝내야 합니다**. 자식이 `main()` 을 리턴하거나 다른 코드를 실행하면 부모 셸의 동작을 복제한 두 개의 셸이 생기는 끔찍한 결과가 생깁니다.

## `wait` 계열 — 자식 수거

자식이 종료해도 **부모가 수거(reap)** 하기 전까지는 커널이 종료 상태를 메타데이터로 남겨둡니다. 이걸 수거하지 않으면 **좀비(zombie)** 프로세스가 됩니다.

### `wait(2)`

```c
#include <sys/wait.h>
pid_t	wait(int *wstatus);
```

- 아무 자식이나 하나가 끝날 때까지 **블록**.
- 자식이 없으면 `-1` 리턴, `errno = ECHILD`.
- 종료 정보를 `wstatus` 에 채움.

### `waitpid(2)`

```c
pid_t	waitpid(pid_t pid, int *wstatus, int options);
```

`wait()` 의 일반화 버전.

| `pid` 값 | 의미 |
|----------|------|
| `> 0` | 해당 PID의 자식만 기다림 |
| `0`   | 같은 프로세스 그룹의 자식 |
| `-1`  | 아무 자식이나 (wait과 동일) |
| `< -1`| 절댓값에 해당하는 프로세스 그룹 |

`options` 에 `WNOHANG` 을 주면 **즉시 리턴**(논블로킹), 수거할 자식이 없으면 0 리턴.

### `wait3` / `wait4`

자원 사용량(`struct rusage`)까지 얻고 싶을 때 씁니다. minishell 에서는 보통 `waitpid` 로 충분합니다.

### 종료 상태 해석 매크로

`wait*` 가 채워준 `status` 는 단순한 정수가 아니라 **비트 필드**입니다. 아래 매크로로 해석합니다.

| 매크로 | 의미 |
|--------|------|
| `WIFEXITED(status)`   | 정상 종료면 참 |
| `WEXITSTATUS(status)` | 정상 종료 코드(0~255) |
| `WIFSIGNALED(status)` | 시그널로 죽었으면 참 |
| `WTERMSIG(status)`    | 죽인 시그널 번호 |
| `WIFSTOPPED(status)`  | 일시 정지됐으면 참 (Ctrl-Z) |
| `WSTOPSIG(status)`    | 정지시킨 시그널 번호 |

```c
int status;
waitpid(pid, &status, 0);

if (WIFEXITED(status))
    last_status = WEXITSTATUS(status);
else if (WIFSIGNALED(status))
{
    int sig = WTERMSIG(status);
    last_status = 128 + sig;  /* bash 규약 */
    if (sig == SIGINT)
        write(STDERR_FILENO, "\n", 1);
    else if (sig == SIGQUIT)
        write(STDERR_FILENO, "Quit (core dumped)\n", 19);
}
```

`$?` 가 가리키는 값이 바로 이 `last_status` 입니다.

### 셸이 종료 코드에 128을 더하는 이유

시그널로 죽은 경우 bash는 `$?` 에 `128 + signo` 를 씁니다. **왜 하필 128인가?** 이건 "하나의 정수에 두 가지 정보를 합치기" 위한 인코딩 규약입니다.

#### 문제의 뿌리

Unix에서 종료 상태는 **0~255 범위의 8-bit 정수 하나**입니다. 그런데 프로세스가 끝나는 방식은 두 가지:

- `exit(N)` 으로 **정상 종료** — 어떤 숫자 N을 전달했는지
- **시그널로 죽음** — 어떤 시그널(2=SIGINT, 9=SIGKILL, 11=SIGSEGV…)에 의해

`wait()` 시스템 콜은 이 둘을 분리해서 보관합니다. 위 "종료 상태 해석 매크로" 표에서 봤듯이 `WIFEXITED`/`WEXITSTATUS` 와 `WIFSIGNALED`/`WTERMSIG` 가 따로 있어요. **커널 레벨에서는 두 정보가 별도**입니다.

즉 합치는 건 **셸이 합니다**.

#### 셸의 고민

셸은 사용자에게 `$?` 라는 **단일 값**을 노출합니다. 사용자가 `echo $?` 하면 정수 하나가 나와야 편합니다. 그러면 "정상 종료 코드 2" 와 "SIGINT(2)로 죽음" 을 같은 `$?` 에서 어떻게 구분할 것인가?

#### 인코딩 규약

0~255 범위를 다음과 같이 쪼갰습니다:

| 범위 | 의미 |
|------|------|
| 0        | 정상 종료 (성공) |
| 1~125    | 프로그램이 정의한 에러 코드 |
| 126      | 파일은 있으나 실행 권한 없음 |
| 127      | command not found |
| 128~255  | 시그널로 죽음 (= 128 + 시그널 번호) |

**왜 128인가?** 8-bit 정수에서 **최상위 비트(bit 7)** 가 켜져 있으면 128 이상입니다. 이게 "시그널로 죽었다" 는 표식.

```
  0 ~ 127 :  0xxxxxxx   (bit 7 = 0) → 정상 종료
128 ~ 255 :  1xxxxxxx   (bit 7 = 1) → 시그널 (하위 7비트가 시그널 번호)
```

#### 구체 예시

```sh
cat                       # Ctrl-C 로 종료
echo $?                   # 130  ← 128 + 2 (SIGINT)

sleep 10                  # 다른 터미널에서 kill -9
echo $?                   # 137  ← 128 + 9 (SIGKILL)

./my_segfault             # 널 포인터 접근
echo $?                   # 139  ← 128 + 11 (SIGSEGV)

bash -c 'exit 130'        # exit(130) 로 나가면?
echo $?                   # 130  ← 시그널 종료와 구분 안 됨
```

마지막 예시가 이 규약의 한계 — `exit(130)` 과 "SIGINT으로 죽음" 은 `$?` 상으로 **구분 불가**입니다. 하지만 관례적으로 프로그램이 `exit(128+)` 로 끝내지 않기로 합의했기 때문에 실무에서는 충돌이 없습니다.

#### 왜 꼭 128이었나? (대안과 비교)

다른 선택지들이 왜 기각됐는지 보면 더 명확해집니다.

| 대안 | 문제 |
|------|------|
| 음수로 표시 (`-2` = SIGINT) | exit 코드는 unsigned 8-bit, 음수 저장 불가 |
| 256 이상 사용 (`1024 + signo`) | 8-bit 범위 초과 — `$?` 에 못 담음 |
| 별도 변수로 (`$SIGNAL`) | 스크립트 ergonomic 저하. `cmd || echo bad` 같은 패턴에서 종료 원인을 하나의 `$?` 로 보는 게 자연스러움 |
| 상위 비트 대신 `200+` 등 | 128(=2^7) 이 **비트 단위로 의미가 뚜렷** — "상위 비트 = 시그널" 이라는 깔끔한 분리 |

#### 셸 구현에서의 적용

셸이 자식을 `waitpid` 로 수거할 때 이 규약을 **직접 구현**해야 합니다. 커널이 알아서 해주지 않습니다.

```c
int status;
waitpid(pid, &status, 0);

if (WIFEXITED(status))
    sh->last_status = WEXITSTATUS(status);          /* 0~255 그대로 */
else if (WIFSIGNALED(status))
    sh->last_status = 128 + WTERMSIG(status);       /* ← 이 규약 */
```

minishell 구현 시 이 공식을 잊으면 `$?` 가 bash와 달라져 테스터 실패의 원인이 됩니다.

#### 정리

- 커널은 "정상 종료 코드" 와 "죽인 시그널" 을 **분리해서** 알려줌 (`WIFEXITED` / `WIFSIGNALED`).
- 셸은 이 둘을 **한 개의 `$?`** 로 합쳐야 함.
- 합치는 규약: **`exit(N)` 은 그대로, 시그널로 죽으면 `128+N`**.
- 128을 고른 이유: 8-bit 정수의 **최상위 비트(bit 7)** 가 곧 "시그널로 죽음" 의 표식.

## 좀비와 고아 프로세스

### 좀비(zombie)
자식이 종료됐지만 부모가 아직 `wait` 하지 않은 상태. `ps` 에서 `Z` 로 표시됩니다.

**수거하지 않은 좀비**는 PID 테이블 엔트리를 점유하므로, 장기 실행 셸에서 `wait` 를 빠뜨리면 PID를 소진할 수 있습니다.

### 고아(orphan)
부모가 먼저 죽었는데 자식이 살아 있는 상태. `init`(PID 1) 이 **자동으로 입양**해서 수거해줍니다. 그래서 고아는 **문제가 아닙니다** — 좀비가 문제입니다.

## 셸에서 fork 가 일어나는 지점

셸이 fork 를 쓰는 전형적 지점:

1. **외부 명령 실행** — 빌트인이 아니면 fork 후 자식에서 `execve()`.
2. **파이프라인** — `cmd1 | cmd2` 에서 두 명령 각각 (N단이면 N번 또는 이진 트리 재귀).
3. **heredoc 본문 수집** — 자식이 사용자 입력을 파이프에 쓰고, 부모가 받음 (Ctrl-C 로 heredoc 만 중단하고 셸은 살리려고).

### 빌트인은 왜 fork 하지 않나?

`cd`, `export`, `exit` 같은 빌트인은 **부모 셸의 상태를 바꿔야** 하므로 자식에서 실행하면 무의미합니다. 단, **파이프라인 안에서**는 예외입니다.

```sh
echo hello | cat   # cat만 중요
cd /tmp            # 셸 상태 변경
echo hi | cd /tmp  # bash는 자식에서 cd 실행 — 부모 cwd는 안 바뀜
```

bash는 **파이프라인 안에서만 빌트인도 자식에서 실행**합니다. minishell도 이를 따라야 합니다. 보통 "자식 프로세스 안에 있는가" 를 나타내는 플래그(예: `is_child`)를 셸 상태에 두고, 이 플래그에 따라 fork 여부를 결정합니다.

## 예제: 외부 명령 하나 실행

fork + execve + waitpid 의 최소 예제.

```c
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    char *argv[] = {"/bin/ls", "-l", NULL};
    char *envp[] = {NULL};
    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        return 1;
    }
    if (pid == 0)
    {
        execve(argv[0], argv, envp);
        perror("execve");
        _exit(127);
    }
    /* parent */
    int status;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
        printf("child exited with %d\n", WEXITSTATUS(status));
    return 0;
}
```

## 자주 하는 실수

### 1. 자식에서 `exit()` 하지 않기
`execve` 가 실패하면 자식이 부모 코드로 돌아가 **셸이 두 벌**이 됩니다. 반드시 `exit()` 또는 `_exit()`.

### 2. 부모에서 파일 디스크립터를 닫지 않기
`pipe()` 후 사용하지 않는 끝을 부모에서 닫지 않으면 파이프 EOF가 오지 않아 **데드락**. [05. 파이프와 IPC](05-파이프와-IPC) 참조.

### 3. `wait` 안 하기
자식을 포크해놓고 `wait` 하지 않으면 좀비가 쌓임. 모든 fork 마다 대응하는 wait가 있어야 함.

### 4. 시그널 핸들러를 fork 직전까지 잘못 상태로 두기
자식에서 실행되는 외부 프로그램은 부모 셸의 SIGINT 무시 설정을 그대로 받을 수 있음. `execve` 직전에 시그널 핸들러를 `SIG_DFL` 로 복원해야 함.

### 5. `fork` 후 `printf` 가 중복 출력
`stdio` 버퍼가 복제되기 때문에, fork 전에 출력한 내용이 **fflush 되지 않은 상태**라면 부모·자식 모두에서 다시 출력됩니다. `fork` 전에 `fflush(stdout)` 하거나, 셸 구현에서는 `write()` 만 쓰는 편이 안전합니다.

## 관련 맨페이지

```sh
man 2 fork
man 2 execve
man 2 wait
man 2 waitpid
```

## 다음 읽을 페이지

- [04. 파일 디스크립터](04-파일-디스크립터) — fork 와 함께 가장 중요한 개념
