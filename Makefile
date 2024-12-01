CC = gcc
CFLAGS = -Wall -g
OBJS = shell.o commands.o signals.o redirection.o
TARGET = shell

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

shell.o: shell.c commands.h signals.h redirection.h
	$(CC) $(CFLAGS) -c shell.c

commands.o: commands.c commands.h
	$(CC) $(CFLAGS) -c commands.c

signals.o: signals.c signals.h
	$(CC) $(CFLAGS) -c signals.c

redirection.o: redirection.c redirection.h
	$(CC) $(CFLAGS) -c redirection.c

clean:
	rm -f $(TARGET) $(OBJS)

