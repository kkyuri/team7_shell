#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "redirection.h"

// 파일 재지향 처리 함수
int handle_redirection(char *args[], int *fd_in, int *fd_out) {
    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], ">") == 0) { // 출력 재지향
            *fd_out = open(args[i + 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
            if (*fd_out < 0) {
                perror("출력 파일을 열 수 없습니다");
                return -1;
            }
            args[i] = NULL; // 재지향 연산자를 명령어에서 제거
        } else if (strcmp(args[i], "<") == 0) { // 입력 재지향
            *fd_in = open(args[i + 1], O_RDONLY);
            if (*fd_in < 0) {
                perror("입력 파일을 열 수 없습니다");
                return -1;
            }
            args[i] = NULL; // 재지향 연산자를 명령어에서 제거
        }
    }
    return 0;
}

// 파이프 처리 함수
int handle_pipe(char *args[]) {
    int pipe_fd[2];   // 파이프 파일 디스크립터 (pipe_fd[0]: 읽기, pipe_fd[1]: 쓰기)
    pid_t pid;
    int pipe_index = -1;

    // 파이프 구분자 '|' 위치 찾기
    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "|") == 0) {
            pipe_index = i;
            break;
        }
    }

    if (pipe_index != -1) {
        pipe(pipe_fd);
        pid = fork();

        if (pid == 0) { // 자식 프로세스
            args[pipe_index] = NULL;
            dup2(pipe_fd[1], STDOUT_FILENO);  // 파이프에 쓰기
            close(pipe_fd[0]);
            close(pipe_fd[1]);
            execvp(args[0], args);
            perror("명령어 실행 오류");
            exit(1);
        } else { // 부모 프로세스
            args += pipe_index + 1;
            wait(NULL);
            dup2(pipe_fd[0], STDIN_FILENO);   // 파이프에서 읽기
            close(pipe_fd[0]);
            close(pipe_fd[1]);
            execvp(args[0], args);
            perror("명령어 실행 오류");
            exit(1);
        }
    }
    return 0;
}

