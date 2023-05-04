#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>

#define MAXLINE 8192
#define MAXBUF 8192
#define RIO_BUFSIZE 8192
#define LISTENQ 1024  // Second argument to listen(). Maximum queue length of pending connections for sockfd.
#define SUCCESS 0
#define FAILURE -1

// Persistent state for the robust I/O (rio) package
typedef struct {
    int rio_fd;                 // Descriptor for this internal buf
    int rio_cnt;                // Unread bytes in internal buf
    char *rio_bufptr;           // Next unread byte in internal buf
    char rio_buf[RIO_BUFSIZE];  // Internal buffer
} rio_t;

void rio_readinitb(rio_t *rp, int fd);
ssize_t rio_writen(int fd, void *usrbuf, size_t n);
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen, bool ignore_new_line);
void serve_contents(int fd);
int open_listen_fd(char *host_name, char *port);

#endif
