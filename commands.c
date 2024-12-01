#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "commands.h"

// ls 명령어
void ls_command() {
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(".")) == NULL) {
        perror("디렉토리를 열 수 없습니다");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }
    closedir(dir);
}

// pwd 명령어
void pwd_command() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("현재 작업 디렉토리를 가져올 수 없습니다");
    } else {
        printf("%s\n", cwd);
    }
}

// cd 명령어
void cd_command(char *path) {
    if (chdir(path) != 0) {
        fprintf(stderr, "디렉토리를 변경할 수 없습니다: %s\n", path);
    }
}

// mkdir 명령어
void mkdir_command(char *name) {
    if (mkdir(name, 0755) != 0) {
        perror("디렉토리를 생성할 수 없습니다");
    }
}

// rmdir 명령어
void rmdir_command(char *name) {
    if (rmdir(name) != 0) {
        perror("디렉토리를 삭제할 수 없습니다");
    }
}

// ln 명령어
void ln_command(char *source, char *target) {
    if (link(source, target) != 0) {
        perror("링크 생성 실패");
    }
}

// cp 명령어
void cp_command(char *source, char *target) {
    FILE *src, *dst;
    char ch;

    src = fopen(source, "r");
    if (!src) {
        perror("파일을 열 수 없습니다");
        return;
    }

    dst = fopen(target, "w");
    if (!dst) {
        perror("파일을 만들 수 없습니다");
        fclose(src);
        return;
    }

    while ((ch = fgetc(src)) != EOF) {
        fputc(ch, dst);
    }

    fclose(src);
    fclose(dst);
}

// rm 명령어
void rm_command(char *name) {
    if (remove(name) != 0) {
        perror("파일 삭제 실패");
    }
}

// mv 명령어
void mv_command(char *source, char *target) {
    if (rename(source, target) != 0) {
        perror("파일 이동 실패");
    }
}

// cat 명령어
void cat_command(char *filename) {
    FILE *file;
    char ch;

    file = fopen(filename, "r");
    if (!file) {
        perror("파일을 열 수 없습니다");
        return;
    }

    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }

    fclose(file);
}

// 명령어 실행 함수
int execute_command(char *args[], int fd_in, int fd_out, int background) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("프로세스를 생성할 수 없습니다");
        return -1;
    }

    if (pid == 0) { // 자식 프로세스
        if (fd_in != -1) dup2(fd_in, STDIN_FILENO);
        if (fd_out != -1) dup2(fd_out, STDOUT_FILENO);

        if (execvp(args[0], args) < 0) {
            fprintf(stderr, "명령어를 실행할 수 없습니다: %s\n", args[0]);
            exit(EXIT_FAILURE);
        }
    }

    // 부모 프로세스에서 백그라운드 실행 여부 처리
    if (!background) {
        waitpid(pid, NULL, 0); // 포그라운드 실행: 자식 프로세스가 끝날 때까지 대기
    } else {
        // 백그라운드에서 실행 중인 프로세스 정보 출력
        printf("[%d] %d\n", getpid(), pid);
        
        // 자식 프로세스 종료 후 좀비 프로세스를 방지하기 위해 SIGCHLD 시그널을 무시
        signal(SIGCHLD, SIG_IGN);
    }

    return 0;
}

