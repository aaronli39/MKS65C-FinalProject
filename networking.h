#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <ctype.h>

#define YEL "\x1B[33m"
#define GRN "\x1B[32m"
#define CYN "\x1B[36m"
#define RED "\x1B[31m"
#define RESET "\x1B[0m"

#ifndef NETWORKING_H
#define NETWORKING_H

#define BUFFER_SIZE 1024
#define PORT "9003"
#define TEST_IP "127.0.0.1"

void error_check(int i, char *s);
int server_setup();
int server_connect(int sd);
int client_setup(char * server);


#endif
