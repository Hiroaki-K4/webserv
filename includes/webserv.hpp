#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>

#define MAXLINE 8192
#define MAXBUF 8192
#define IO_BUFSIZE 8192
#define LISTENQ 1024  // Second argument to listen(). Maximum queue length of pending connections for sockfd.
#define SUCCESS 0
#define FAILURE -1

typedef struct {
    int io_fd;  // Descriptor for this internal buf
    int read_count;
    char *io_buf_ptr;         // Next unread byte in internal buf
    char io_buf[IO_BUFSIZE];  // Internal buffer
} io;

void io_readinitb(io *io_s, int fd);
ssize_t io_writen(int fd, void *input, size_t len);
ssize_t io_readlineb(io *rp, void *usrbuf, size_t maxlen, bool ignore_new_line);
void serve_contents(int fd);
int open_listen_fd(char *host_name, char *port);

#endif
