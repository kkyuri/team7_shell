#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// SIGINT (Ctrl-C) 신호를 처리할 핸들러 함수
void sigint_handler(int signum) {
    // SIGINT가 들어오면 메시지를 출력하고 기본 동작(프로세스 종료)을 실행
    printf("\n프로세스를 종료합니다.\n");
    exit(0);  // 프로그램 종료
}

// SIGQUIT (Ctrl-\) 신호를 처리할 핸들러 함수
void sigquit_handler(int signum) {
    // SIGQUIT가 들어오면 메시지를 출력하고 기본 동작(프로세스 종료)을 실행
    printf("\n프로세스를 종료합니다.\n");
    exit(0);  // 프로그램 종료
}

// SIGTSTP (Ctrl-Z) 신호를 처리할 핸들러 함수
void sigtstp_handler(int signum) {
    // SIGTSTP가 들어오면 메시지를 출력하고 기본 동작(프로세스를 일시 정지)을 그대로 사용
    printf("\n프로세스가 일시 정지되었습니다. (Ctrl-Z)\n");
    // 아무런 추가 작업을 하지 않으면 기본적으로 프로세스가 일시 정지됨
}

// 신호 처리기 설정 함수
void setup_signal_handlers() {
    // SIGINT, SIGQUIT, SIGTSTP 신호에 대한 핸들러 설정
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, sigquit_handler);
    signal(SIGTSTP, sigtstp_handler);
}

