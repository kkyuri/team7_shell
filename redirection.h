#ifndef REDIRECTION_H
#define REDIRECTION_H

int handle_redirection(char *args[], int *fd_in, int *fd_out);
int handle_pipe(char *args[]);

#endif

