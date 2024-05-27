#ifndef _STD_H
#define _STD_H

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define fatal(message)      \
    do {                    \
        perror(message);    \
        exit(EXIT_FAILURE); \
    } while (0)

#endif
