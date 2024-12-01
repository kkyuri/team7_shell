// signals.h
#ifndef SIGNALS_H
#define SIGNALS_H

void handle_sigint(int sig);
void handle_sigquit(int sig);
void setup_signals();

#endif

