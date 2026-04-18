# 05. 파이프와 IPC

## IPC가 뭐고 왜 필요한가

**IPC(Inter-Process Communication)** 는 서로 다른 프로세스가 **데이터를 주고받는 메커니즘**입니다. 프로세스는 기본적으로 메모리가 격리돼 있어서 직접 변수를 공유할 수 없습니다. 그래서 커널이 다음과 같은 통신 수단을 제공합니다.

- **파이프(pipe)** — 파일 디스크립터로 접근하는 단방향 바이트 스트림. 부모·자식 간 통신.
- **FIFO(named pipe)** — 이름 있는 파이프. 관계없는 프로세스 사이에도 쓸 수 있음.
- **시그널(signal)** — 짧은 비동기 알림. [06. 시그널 처리](06-시그널-처리).
- **공유 메모리, 세마포어, 메시지 큐, 소켓** 등.

셸에서 가장 많이 쓰는 건 **파이프**입니다. `cmd1 | cmd2` 의 `|` 가 바로 파이프입니다.

## `pipe(2)` — 익명 파이프 생성

```c
#include <unistd.h>
int	pipe(int pipefd[2]);
```

호출하면 **커널 안에 FIFO 큐**가 하나 생기고, 두 개의 fd가 프로세스에게 반환됩니다.

| 인덱스 | 방향 |
|--------|------|
| `pipefd[0]` | **읽기** 끝 (read end) |
| `pipefd[1]` | **쓰기** 끝 (write end) |

```c
int fds[2];
if (pipe(fds) == -1)
{
    perror("pipe");
    return 1;
}
write(fds[1], "hello", 5);
char buf[16];
ssize_t n = read(fds[0], buf, sizeof(buf));
/* buf 에 "hello", n == 5 */
```

**기억하기**: 인덱스 0이 읽기(read), 인덱스 1이 쓰기(write). 외우는 방법은 **"0은 stdin, 1은 stdout"** 처럼 생각하면 됩니다. stdin은 읽는 곳이니 pipefd[0]이 읽기, stdout은 쓰는 곳이니 pipefd[1]이 쓰기.

### 파이프의 특성

1. **단방향**. 양방향이 필요하면 pipe 두 개.
2. **버퍼 크기 제한**. 리눅스 기본 64KB. 넘치면 writer가 블록.
3. **EOF는 모든 writer가 write 끝을 닫아야 발생**. 여기가 가장 중요.
4. 모든 reader가 read 끝을 닫은 상태에서 write하면 **SIGPIPE** 시그널이 발생(기본 동작: 프로세스 종료).

### EOF 전달 규칙

파이프의 **read 끝**에서 `read()` 가 `0`(EOF)을 돌려주려면:

> **모든 프로세스**에서 write 끝(fd)이 **모두 닫혀야** 함.

하나라도 열려 있으면 `read()` 는 블록됩니다. 이게 파이프라인 구현에서 가장 많이 실수하는 부분.

## 부모-자식 간 파이프 통신

```
[부모]
  pipe(fds)
    │
    ▼
  fork()
    ├──────────────────────┐
  [부모]              [자식]
  fds[0] read        fds[0] read       ← fork으로 같은 fd가 복제됨
  fds[1] write       fds[1] write
```

fork 후 **부모·자식 모두 양쪽 fd를 가진 상태**입니다. 이걸 정리해서 단방향 통신을 만듭니다.

### 부모가 쓰고 자식이 읽는 예

```c
int fds[2];
pipe(fds);

pid_t pid = fork();
if (pid == 0)
{
    /* 자식: 읽기만 할 거니까 쓰기 끝은 닫는다 */
    close(fds[1]);

    char buf[64];
    ssize_t n = read(fds[0], buf, sizeof(buf));
    write(STDOUT_FILENO, buf, n);

    close(fds[0]);
    _exit(0);
}
/* 부모: 쓰기만 할 거니까 읽기 끝은 닫는다 */
close(fds[0]);

write(fds[1], "hello from parent\n", 18);
close(fds[1]);  /* 닫아야 자식이 EOF를 봄 */

wait(NULL);
```

**`close(fds[1])` 를 빠뜨리면** 자식의 `read()` 가 영원히 블록됩니다. 부모의 write 끝이 아직 열려있으니 커널은 "또 누가 쓸 수도 있다"고 판단해 EOF를 주지 않는 것.

## 셸 파이프라인 구현 (핵심 알고리즘)

`cmd1 | cmd2` 를 구현해봅시다. 목표:

- `cmd1` 의 stdout이 `cmd2` 의 stdin으로 연결.
- 둘 다 끝날 때까지 기다림.
- 최종 종료 상태는 **오른쪽(cmd2) 의 종료 상태**(bash 규약).

```c
int	run_pipeline(char **cmd1, char **cmd2)
{
    int pipefd[2];
    pipe(pipefd);

    pid_t left = fork();
    if (left == 0)
    {
        /* 왼쪽: stdout → pipe write */
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);  /* 필요 없음 */
        close(pipefd[1]);  /* dup2 했으니 원본도 닫음 */
        execve(cmd1[0], cmd1, envp);
        _exit(127);
    }

    pid_t right = fork();
    if (right == 0)
    {
        /* 오른쪽: stdin ← pipe read */
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
        execve(cmd2[0], cmd2, envp);
        _exit(127);
    }

    /* 부모: 양쪽 다 닫아야 자식 파이프에 EOF 전달 */
    close(pipefd[0]);
    close(pipefd[1]);

    int status;
    waitpid(left,  NULL,    0);
    waitpid(right, &status, 0);

    return (WIFEXITED(status) ? WEXITSTATUS(status) : 128 + WTERMSIG(status));
}
```

### 왜 `close()` 를 네 번이나 하나?

파이프라인에서 `close` 를 빠뜨리기 쉽습니다. 체크리스트:

```
자식1:  close(pipefd[0])   ← 자기가 안 쓰는 쪽
        close(pipefd[1])   ← dup2 한 원본
자식2:  close(pipefd[0])   ← dup2 한 원본
        close(pipefd[1])   ← 자기가 안 쓰는 쪽
부모:   close(pipefd[0])   ← 부모도 안 씀
        close(pipefd[1])   ← 부모도 안 씀
```

**6번의 close가 필요**합니다(각 프로세스에서 2번씩, 부모 포함). 하나라도 빠지면 다음 증상:

| 빠뜨린 쪽 | 증상 |
|-----------|------|
| 자식1의 read 쪽 | 즉시 문제는 없지만 fd 누수 |
| 자식2의 write 쪽 | 즉시 문제는 없지만 fd 누수 |
| 자식1의 write 원본 | `cmd1` 가 끝나도 파이프 EOF가 안 생겨 `cmd2` 가 블록됨 |
| 자식2의 read 원본 | `cmd2` 프로세스 안에서 fd 누수 |
| 부모의 read | 자식2가 EOF를 못 받거나 부모에 남아있는 fd 때문에 헷갈림 |
| **부모의 write** | **`cmd1` 이 죽어도 파이프 write 끝이 부모에 남아있으므로 `cmd2` 가 read에서 영원히 블록** |

## N단 파이프라인 (3개 이상)

`cmd1 | cmd2 | cmd3` 처럼 3개 이상일 때는 두 가지 구조가 가능합니다.

### 1. 루프 기반 (N개 pipe, N+1 fork)
```c
int prev_read = -1;
for (i = 0; i < ncmds; i++)
{
    int fds[2];
    if (i < ncmds - 1) pipe(fds);

    pid_t pid = fork();
    if (pid == 0)
    {
        if (prev_read != -1)
        {
            dup2(prev_read, STDIN_FILENO);
            close(prev_read);
        }
        if (i < ncmds - 1)
        {
            dup2(fds[1], STDOUT_FILENO);
            close(fds[0]); close(fds[1]);
        }
        execve(cmd[i][0], cmd[i], envp);
        _exit(127);
    }

    if (prev_read != -1) close(prev_read);
    if (i < ncmds - 1)
    {
        close(fds[1]);
        prev_read = fds[0];
    }
}
while (wait(NULL) > 0);
```

### 2. 이진 트리 기반 (AST 재귀)
파이프라인을 왼쪽·오른쪽으로 쪼갠 이진 트리로 표현하고, 각 노드에서 fork 두 번. 재귀적으로 깔끔하다는 장점.

```
cmd1 | cmd2 | cmd3
        │
        ▼
     PIPELINE
     /      \
   cmd1  PIPELINE
          /    \
        cmd2  cmd3
```

- N 깊이의 파이프라인에서 중간 프로세스가 N개 이상 생길 수 있다는 점은 감안할 것.
- 파서가 이미 이진 트리를 만들어주면 평가기가 자연스럽게 재귀만 돌면 됨.

## SIGPIPE

**reader가 이미 모두 죽은 파이프에 write** 하면 커널이 `SIGPIPE` 시그널을 보냅니다. 기본 동작은 **프로세스 종료**.

```sh
yes | head -n 3   # head 3줄 출력 후 종료. yes는 SIGPIPE로 죽음
```

셸에서는 보통 기본 동작을 그대로 둡니다(서브젝트에서도 특별히 처리 요구하지 않음). `write` 의 반환값을 체크해 `errno == EPIPE` 로 처리할 수도 있지만, minishell 에서는 기본 동작 허용이 깔끔합니다.

## 파이프와 리다이렉션의 상호작용

`cmd1 < infile | cmd2 > outfile` 의 처리 순서:

```
cmd1: stdin ← infile, stdout ← pipe write
cmd2: stdin ← pipe read, stdout ← outfile
```

각 자식 프로세스 안에서 이 순서로 처리:

1. 파이프 dup2 먼저(pipeline 설정).
2. 그 다음 리다이렉션 dup2(파이프보다 우선).

그래서 `cmd1 > file | cmd2` 에서 `cmd1` 의 출력은 파이프가 아닌 `file` 로 갑니다. bash와 동일.

평가 단계에서는 "명령 정보를 `t_cmd_form` 같은 구조체에 채운 뒤, prefix · suffix 의 리다이렉션을 순서대로 적용" 하는 순서가 자연스럽습니다.

## heredoc — 파이프의 재미있는 활용

`<< DELIM` 은 사용자 입력을 특정 구분자가 나올 때까지 수집해서 stdin으로 넣어주는 기능입니다.

구현 패턴:

1. 파이프 하나 생성.
2. **부모(또는 fork 한 child)** 가 사용자 입력을 한 줄씩 읽어 `delim` 에 도달할 때까지 파이프에 write.
3. `write 끝을 close` 해서 EOF 전달.
4. 명령 실행 시 `read 끝을 dup2(STDIN_FILENO)`.

```c
int heredoc_pipe[2];
pipe(heredoc_pipe);

pid_t pid = fork();
if (pid == 0)
{
    /* 자식이 heredoc 수집 전담 */
    init_heredoc_signal();  /* Ctrl-C 처리 */
    close(heredoc_pipe[0]);

    while ((line = readline("> ")) != NULL)
    {
        if (strcmp(line, delim) == 0)
            break;
        /* $VAR 확장 (delim이 안 quoted였다면) */
        write(heredoc_pipe[1], line, strlen(line));
        write(heredoc_pipe[1], "\n", 1);
        free(line);
    }
    close(heredoc_pipe[1]);
    _exit(0);
}
close(heredoc_pipe[1]);  /* 부모는 읽기만 할 것 */
waitpid(pid, NULL, 0);

/* 이제 heredoc_pipe[0] 을 명령의 stdin으로 dup2 */
dup2(heredoc_pipe[0], STDIN_FILENO);
close(heredoc_pipe[0]);
```

**2패스 변형**: AST 전체를 평가 전에 한 번 훑어 모든 heredoc 본문을 미리 수집하고, 실제 명령 평가 때는 이미 준비된 파이프/파일을 stdin 으로 연결만 하는 구조도 흔합니다. bash 도 파싱 직후 미리 수집합니다. 장점은 `cmd1 && cat << EOF` 에서 `cmd1` 성공 여부와 무관하게 heredoc 입력이 자연스럽게 끝나는 것.

## 파이프 디스크립터 관리 체크리스트

파이프라인 구현할 때 머릿속에 이 그림을 그리면 좋습니다.

```
pipe(fds) 직후:
  부모: [0_r, 1_w] [fds[0]_r, fds[1]_w]

fork() 직후 (부모, 자식 둘 다 같음):
  [0_r, 1_w] [fds[0]_r, fds[1]_w]

자식에서 dup2(fds[1], 1):
  [0_r, fds[1]_w] [fds[0]_r, fds[1]_w]
                   ↑
                   이건 닫아야 함 — 안 닫으면 누수
```

외우는 규칙:

- `dup2` 한 원본은 **항상 close**.
- 내가 안 쓰는 끝도 **항상 close**.
- 부모가 자식에게 파이프를 넘겼다면 **부모도 양쪽 close**.

## 자주 하는 실수

### 1. 부모가 파이프를 안 닫는다
"자식에게 넘겼으니 부모는 신경 안 써도 되지 않을까?" → **안 됩니다**. fd 복제 개념을 기억할 것.

### 2. `pipe()` 를 fork 뒤에 부른다
fork 뒤에 부모와 자식이 각자 `pipe()` 를 부르면 **서로 다른 파이프**가 생겨 통신이 안 됩니다. 반드시 **fork 전에** 한 번만.

### 3. 파이프 읽기/쓰기 방향 혼동
`pipefd[0] = read`, `pipefd[1] = write`. 0-1 위치를 계속 헷갈리면 메모에 써두기.

### 4. 파이프라인 종료 상태를 왼쪽 것으로 집계
bash는 **가장 오른쪽** 명령의 exit를 `$?` 로 씁니다. `set -o pipefail` 옵션을 켜야 왼쪽 실패도 반영되는데, minishell은 pipefail 구현 안 해도 됩니다.

### 5. 중간에 있는 명령은 양쪽 파이프 모두 처리해야 함
N단 파이프라인에서 가운데 명령은 **stdin도 이전 파이프, stdout도 다음 파이프**. 빠뜨리기 쉽습니다.

## 관련 맨페이지

```sh
man 2 pipe
man 7 pipe   # 파이프의 EOF 규칙 등 포괄적 설명
```

## 다음 읽을 페이지

- [06. 시그널 처리](06-시그널-처리)
