# 13. PATH 검색과 실행

## `execve` 는 PATH 를 모른다

[03. 프로세스와 fork](03-프로세스와-fork) 에서 봤듯 `execve(2)` 는 **경로를 직접 받아야** 합니다.

```c
execve("/bin/ls", argv, envp);   /* OK */
execve("ls",       argv, envp);   /* -1, ENOENT */
```

`ls` 만 치면 `/bin/ls` 인지 `/usr/local/bin/ls` 인지 커널은 알 방법이 없습니다. **셸이 `PATH` 를 뒤져서 전체 경로를 찾아 넘겨야** 합니다.

### `execlp`, `execvp` — PATH 검색을 해주는 친구들

libc 에는 PATH 검색까지 해주는 `execlp/execvp` 가 있지만, **서브젝트가 허용하지 않습니다**. `execve` 만 써야 하므로 PATH 검색을 직접 구현해야 합니다.

## 환경 변수 `PATH`

```sh
echo $PATH
# /usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin
```

콜론(`:`) 으로 구분된 디렉터리 목록. 셸은 왼쪽부터 순서대로 훑어 **처음으로 실행 가능한 파일**을 씁니다.

## 명령 분류

사용자가 입력한 `cmd` 를 다음과 같이 분류합니다.

### 1. `/` 를 포함하는 경로

절대 경로(`/bin/ls`) 또는 상대 경로(`./a.out`, `../tools/sh`). **PATH 검색 안 함**. 그대로 `execve` 에 넘김.

### 2. 슬래시 없는 순수 이름

`ls`, `grep`, `python` 처럼 `/` 없는 이름. **PATH 검색 수행**.

### 3. 빌트인

[12. 빌트인 커맨드](12-빌트인-커맨드) 에 정의된 7개. `execve` 로 가지 않고 셸 내부에서 처리.

### 구현 순서

```
if (is_builtin(cmd)):
    execute_builtin()
elif (contains '/'):
    try execve(cmd, args, envp)
    if 실패: perror, last_status = 127 or 126
else:
    path = find_in_path(cmd, PATH)
    if (path is NULL):
        error "command not found", last_status = 127
    else:
        execve(path, args, envp)
        if 실패: perror, last_status = 126 or 127
```

## PATH 검색 알고리즘

```c
char	*find_path(char *cmd, char *envp[])
{
    char *path_var = find_env(envp, "PATH");
    if (!path_var) return NULL;

    char **dirs = ft_split(path_var, ':');
    if (!dirs) return NULL;

    for (int i = 0; dirs[i]; i++)
    {
        char *candidate = ft_multiplejoin(dirs[i], "/", cmd);
        if (access(candidate, X_OK) == 0)
        {
            free_split(dirs);
            return candidate;
        }
        free(candidate);
    }
    free_split(dirs);
    return NULL;
}
```

### `access(path, X_OK)`

- 실행 권한이 있으면 0 리턴.
- 없으면 -1, `errno = EACCES` 또는 `ENOENT`.

단, `access` 는 **파일 존재 + 권한 확인** 을 한 번에 합니다.

### PATH 가 비어있거나 정의 안 된 경우

```sh
unset PATH
ls                # ls: command not found
```

- `$PATH` 가 없으면 bash 는 **현재 디렉터리에서도 검색 안 함** (기본 PATH fallback 없음, POSIX 는 구현마다 다름).
- minishell 은 PATH 미정의 시 곧바로 "command not found" 로 처리하면 됩니다.

### PATH 에 빈 항목 `::`

```sh
PATH="/bin::/usr/bin"   # 가운데 빈 항목
```

빈 항목은 **현재 디렉터리** 로 해석됩니다(POSIX). `ft_split(':')` 으로 자르면 빈 문자열이 리턴되는데, 이걸 `""/cmd` 로 붙이면 `/cmd` 가 돼서 절대 경로가 됨 — 원하는 동작과 다름.

엄밀한 구현은 빈 항목을 `.` 로 치환하거나, join 시 빈 항목이면 `cmd` 단독 사용. minishell 은 이 엣지 케이스를 깊게 구현 안 해도 됩니다.

## 종료 코드 관례

bash 의 종료 코드 규약:

| 코드 | 의미 |
|------|------|
| 0   | 정상 종료 |
| 1~125 | 명령이 설정한 에러 |
| **126** | 파일은 있으나 실행 권한 없음 또는 디렉터리 |
| **127** | command not found |
| 128+N | 시그널 N 으로 죽음 |

```sh
/etc/hosts        # 존재하지만 실행 파일 아님
# bash: /etc/hosts: Permission denied
echo $?           # 126

nonexistent_cmd
# bash: nonexistent_cmd: command not found
echo $?           # 127
```

minishell 구현도 이 규약을 따라야 하므로 `PERMISSION_DENIED_CODE = 126`, `COMMAND_NOT_FOUND_CODE = 127` 같은 상수를 정의해두면 편리합니다.

## 에러 메시지 포맷

bash 스타일:

```
minishell: cmdname: No such file or directory
minishell: cmdname: Permission denied
minishell: cmdname: command not found
```

`perror("cmdname")` 을 쓰면 `cmdname: No such file or directory` 로 나오므로, 접두어 `"minishell: "` 만 추가하면 됩니다.

```c
ft_putstr_fd("minishell: ", STDERR_FILENO);
perror(cmd);     /* cmd: reason\n */
```

## 실행 시퀀스 (자식 프로세스 안에서)

```c
void	execute_command(t_cmd_form *form)
{
    restore_signal();      /* SIG_DFL 복원 */

    char *path;
    if (ft_strchr(form->cmd, '/'))
    {
        path = form->cmd;                 /* 경로 포함 */
    }
    else
    {
        path = find_path(form->cmd, form->envp);
        if (!path)
        {
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(form->cmd, STDERR_FILENO);
            ft_putendl_fd(": command not found", STDERR_FILENO);
            _exit(127);
        }
    }

    if (access(path, F_OK) == -1)
    {
        ft_putstr_fd("minishell: ", STDERR_FILENO);
        ft_putstr_fd(path, STDERR_FILENO);
        ft_putendl_fd(": No such file or directory", STDERR_FILENO);
        _exit(127);
    }
    if (access(path, X_OK) == -1)
    {
        ft_putstr_fd("minishell: ", STDERR_FILENO);
        ft_putstr_fd(path, STDERR_FILENO);
        ft_putendl_fd(": Permission denied", STDERR_FILENO);
        _exit(126);
    }

    execve(path, form->args, form->envp);

    /* execve 실패한 경우만 여기 도달 */
    perror(path);
    _exit(126);
}
```

### 시그널 복원 순서

`restore_signal()` 은 `SIGINT`, `SIGQUIT` 를 `SIG_DFL` 로 되돌립니다. `execve` 전에 반드시 호출해야 자식이 Ctrl-C/Ctrl-\\ 에 반응. 안 하면 셸이 설정한 "무시" 핸들러가 상속돼 명령이 시그널에 둔감해짐.

### 절대/상대 경로 검증 순서

위 예시는 `access(F_OK)` 로 존재 여부를 먼저 검사하고 `access(X_OK)` 로 실행 권한 검사. 이렇게 두 번 나눠 검사하면 bash 호환 에러 메시지를 구분해 출력하기 쉬워집니다.

## 디렉터리 실행 시도

```sh
/tmp/              # 디렉터리를 실행하려 하면
# bash: /tmp/: Is a directory
echo $?           # 126
```

디렉터리를 `execve` 에 넘기면 `errno = EACCES` 로 실패하고 "Is a directory" 를 명시적으로 출력해야 bash 와 호환. `stat`/`S_ISDIR` 로 먼저 검사하면 정확히 구분 가능.

디렉터리 구분까지 안 해도 perror 메시지로 충분한 피드백을 주면 OK. 완벽을 기하려면 `stat` 으로 먼저 검사해 메시지를 bash 와 동일하게 맞추면 됩니다.

## envp 를 execve 에 어떻게 넘기는가

`execve` 의 세 번째 인자는 `char **envp` — `"KEY=VALUE"` 스트링 배열에 `NULL` 종료.

셸이 **환경 변수를 내부적으로 해시맵**에 저장한다면, execve 직전에 해시맵 → `char**` 로 재구성해야 합니다:

```c
char	**materialize_envp(t_hash_map *map)
{
    int count = count_entry(map);
    char **result = malloc(sizeof(char *) * (count + 1));
    int idx = 0;

    for (int bucket = 0; bucket < DEFAULT_HASHTABLE_SIZE; bucket++)
    {
        t_hash_entry *entry = map->table[bucket];
        while (entry)
        {
            result[idx++] = ft_multiplejoin(entry->key, "=", entry->value);
            entry = entry->next;
        }
    }
    result[idx] = NULL;
    return result;
}
```

### prefix assignment 적용

```sh
FOO=bar LD_PRELOAD=lib.so ./program
```

이 경우 `FOO=bar`, `LD_PRELOAD=lib.so` 는 **자식에게만** 적용되어야 합니다. 부모 셸의 환경은 그대로.

구현:

1. 해시맵 → `char**` 복제 (또는 현재 envp 복사).
2. AST의 `cmd_prefix` 를 훑어 각 `ASSIGN_WORD` 를 배열에 추가 또는 덮어쓰기.
3. 이 수정된 `envp` 를 execve 에 넘김.

**중요**: 부모 셸의 해시맵/환경은 건드리지 않고, 자식에게 넘길 배열만 수정해야 "cmd 가 끝나면 변수도 사라진다" 는 의미가 지켜집니다.

## 특수 케이스

### 빈 인자

```sh
""              # bash: command not found
```

빈 문자열을 명령으로 넘기면 "command not found" 로 처리. 별도 분기 없이 find_path 실패 경로에 태우면 자동 처리됩니다.

### cmd_word 가 없을 때

```sh
FOO=bar         # 환경 변수 선언만 (cmd 없음)
```

- cmd_word 가 NULL 이면 execve 호출 안 함.
- 대신 `FOO=bar` 를 **현재 셸의 환경**에 적용 (bash 규칙).
- last_status = 0.

단독 할당 경로를 빼먹기 쉬우니 평가 단계에서 **"cmd_word 가 NULL 이면 prefix 의 할당을 셸 환경에 영구 적용"** 분기를 명시적으로 두세요.

### 권한은 있는데 실행 불가 (ELF 손상 등)

`execve` 가 `ENOEXEC` 로 실패. bash 는 **스크립트로 시도** (`#!` 없어도 sh 로 실행) 하지만, minishell 은 그냥 perror 출력하고 126/127 으로 종료하면 됩니다.

## 자주 하는 실수

### 1. PATH 검색 후 경로에 `/` 중복
`/usr/bin/` + `/ls` 같은 실수 — 슬래시 중복. join 전에 디렉터리 끝 슬래시 체크.

### 2. `access(X_OK)` 만 보고 `F_OK` 안 봄
존재 안 하는 파일도 access(X_OK)=-1 이지만 에러 메시지 다름. 분리 확인.

### 3. 자식에서 `exit` 대신 `return`
execve 실패 후 return 하면 자식이 부모 코드로 돌아가 셸이 두 벌. `_exit(N)` 필수.

### 4. exit 코드를 `$?` 에 안 반영
파이프라인에서 waitpid 의 status 를 WEXITSTATUS 나 128+WTERMSIG 으로 정확히 변환해야 `$?` 가 맞음.

### 5. 시그널 복원 누락
`restore_signal()` 을 execve 직전에 호출 안 하면 외부 명령이 SIGINT 에 반응 안 함. 대표적인 "cat 에서 Ctrl-C 가 안 먹혀요" 버그.

### 6. PATH 의 상대 경로 해석
`PATH=".:/usr/bin"` 에서 `.` 은 현재 디렉터리. `./ls` 와 `ls` 가 다른 결과를 낼 수 있음.

### 7. PATH 검색 결과 메모리 관리
검색 성공 시 `find_path` 가 **새로 할당**한 경로를 리턴 — 사용 후 free. execve 로 넘겨서 프로세스 교체되면 해제 의미 없지만, execve 실패 시엔 해제 필요. 자식은 어차피 `_exit` 으로 나가니 괜찮다고 할 수 있지만, 깔끔하게 free 하는 습관 권장.

## 다음 읽을 페이지

- [14. 메모리 관리](14-메모리-관리)
