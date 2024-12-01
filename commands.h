#ifndef COMMANDS_H
#define COMMANDS_H

void ls_command();
void pwd_command();
void cd_command(char *path);
void mkdir_command(char *name);
void rmdir_command(char *name);
void ln_command(char *source, char *target);
void cp_command(char *source, char *target);
void rm_command(char *name);
void mv_command(char *source, char *target);
void cat_command(char *filename);

int execute_command(char *args[], int fd_in, int fd_out, int background);

// handle_pipe 함수 선언 추가
int handle_pipe(char *args[]);

#endif

