#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "commands.h"
#include "signals.h"
#include "redirection.h"

#define MAX_INPUT 1024
#define MAX_ARGS 100

// setup_signal_handlers 함수 프로토타입 추가
void setup_signal_handlers();  // 함수 프로토타입

void parse_input(char *input, char *args[], int *background);

int main() {
    char input[MAX_INPUT];
    char *args[MAX_ARGS];
    int background;
    int fd_in = -1, fd_out = -1;

    // 시그널 처리 등록
    setup_signal_handlers();

    while (1) {
        printf("shell> ");
        fflush(stdout);

        if (fgets(input, MAX_INPUT, stdin) == NULL) {
            printf("\n입력을 받을 수 없습니다. 종료합니다.\n");
            break;
        }

        // 개행문자 제거
        input[strcspn(input, "\n")] = 0;

        // "exit" 명령 처리
        if (strcmp(input, "exit") == 0) {
            printf("쉘을 종료합니다.\n");
            break;
        }

        // 사용자 입력 파싱
        parse_input(input, args, &background);
        // 파일 재지향 처리
        if (handle_redirection(args, &fd_in, &fd_out) < 0) {
            continue;
        }

        // 파이프 처리
        if (handle_pipe(args) == 0) {
            // 파이프가 없을 경우 일반 명령어 실행
            if (execute_command(args, fd_in, fd_out, background) < 0) {
                fprintf(stderr, "명령어 실행 중 문제가 발생했습니다.\n");
            }
        }

        // 파일 디스크립터 닫기
        if (fd_in != -1) close(fd_in);
        if (fd_out != -1) close(fd_out);
    }

    return 0;
}

void parse_input(char *input, char *args[], int *background) {
    int i = 0;
    char *token = strtok(input, " ");

    *background = 0;

    while (token != NULL) {
        if (strcmp(token, "&") == 0) {
            *background = 1; // 백그라운드 실행 플래그 설정
        } else {
            args[i++] = token;
        }
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
}
