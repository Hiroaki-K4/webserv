/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 22:12:06 by hkubo             #+#    #+#             */
/*   Updated: 2023/02/11 17:20:24 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int open_listen_fd(char *port)
{
    int listen_fd, opt_val = 1;
    char buf[20];
    struct addrinfo hints, *listp, *p;

    std::memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM; // Ordered and reliable.
    // When specifying multiple flags in ai_flags, specify them by bitwise OR them.
    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG; // AI_PASSIVE->server
    hints.ai_flags = AI_NUMERICSERV; // port should be string include port number.
    getaddrinfo(NULL, port, &hints, &listp);

    for (p = listp; p; p = p->ai_next) {
        if ((listen_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
            continue;
        setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, static_cast<const void *>(&opt_val), sizeof(int));

        // bind function ask the kernel to relate server socket address and socket descriptor(listen_fd).
        if (bind(listen_fd, p->ai_addr, p->ai_addrlen) == 0)
        {
            getnameinfo(p->ai_addr, p->ai_addrlen, buf, 20, NULL, 0, NI_NUMERICHOST);
            std::cout << "Host address: " << buf << std::endl;
            break;
        }
        close(listen_fd);
    }

    freeaddrinfo(listp);
    if (!p) {
        std::cout << "[main.cpp open_listen_fd][ERROR] freeaddrinfo error" << std::endl;
        return -1;
    }
    // Change listen_Fd from active socket to listen socket(wait connection request from client).
    if (listen(listen_fd, LISTENQ) < 0) {
        std::cout << "[main.cpp open_listen_fd][ERROR] listen error" << std::endl;
        close(listen_fd);
        return -1;
    }

    return listen_fd;
}

int parse_uri(char *uri, char *filename, char *cgiargs)
{
    char *ptr;

    if (!strstr(uri, "cgi-bin")) { // Static content
        strcpy(cgiargs, "");
        strcpy(filename, "contents");
        strcat(filename, uri);
        if (uri[strlen(uri) - 1] == '/')
            strcat(filename, "home.html");
        return 1;
    }
    else {
        ptr = index(uri, '?');
        if (ptr) {
            strcpy(cgiargs, ptr + 1);
            *ptr = '\0';  // Change ? to null terminator
        } else {
            strcpy(cgiargs, "");
        }
        strcpy(filename, "./contents");
        strcat(filename, uri);
        return 0;
    }
}

void get_filetype(char *filename, char *filetype)
{
    if (strstr(filename, ".html"))
        strcpy(filetype, "text/html");
    else if (strstr(filename, ".gif"))
        strcpy(filetype, "image/gif");
    else if (strstr(filename, ".png"))
        strcpy(filetype, "image/png");
    else if (strstr(filename, ".jpg"))
        strcpy(filetype, "image/jpeg");
    else
        strcpy(filetype, "text/plain");
}

void serve_static(int fd, char *filename, int filesize)
{
    int srcfd;
    char *srcp, filetype[MAXLINE], buf[MAXBUF];

    // Send response headers to client
    get_filetype(filename, filetype);
    sprintf(buf, "HTTP/1.0, 200 OK\r\n");
    // sprintf(buf, "%sServer: Tiny Web Server\r\n", buf);
    // sprintf(buf, "%sConnection: close\r\n", buf);
    // sprintf(buf, "%sContent-length: %d\r\n", buf, filesize);
    // sprintf(buf, "%sContent-type: %s\r\n", buf, filetype);
    if (rio_writen(fd, buf, strlen(buf)) == -1) {
        printf("rio_writen error!");
        return;
    }
    printf("Response headers:\n");
    printf("%s", buf);

    // Send response body to client
    srcfd = open(filename, O_RDONLY, 0);
    srcp = static_cast<char *>(mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0));
    close(srcfd);
    rio_writen(fd, srcp, filesize);
    munmap(srcp, filesize);
}

void serve_dynamic(int fd, char *filename, char *cgiargs)
{
    char buf[MAXLINE], *emptylist[] = { NULL };

    std::cout << "fd: " << fd << std::endl;
    // Return first part of HTTP response
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Server: Tiny Web Server\r\n");
    rio_writen(fd, buf, strlen(buf));

    if (fork()== 0) {
        setenv("QUERY_STRING", cgiargs, 1);
        dup2(fd, STDOUT_FILENO);
        execve(filename, emptylist, environ);
    }
    wait(NULL);
}

void serve_contents(int fd)
{
    int is_static;
    struct stat sbuf;
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char filename[MAXLINE], cgiargs[MAXLINE];
    rio_t rio;

    // Read request line and headers
    rio_readinitb(&rio, fd);
    rio_readlineb(&rio, buf, MAXLINE, true);
    printf("Request headers:\n");
    printf("%s", buf);
    sscanf(buf, "%s %s %s", method, uri, version);
    printf("method: %s uri: %s version: %s\n", method, uri, version);
    int cmp_res = strcasecmp(method, "GET");
    if (cmp_res != 0) {
        // clienterror(fd, method, "501", "Not implemented", "Tiny does not implement this method");
        printf("Tiny does not implement this method\n");
        return;
    }
    // read_requesthdrs(&rio);

    // Parse URI from GET request
    is_static = parse_uri(uri, filename, cgiargs);
    printf("is_static: %d filename: %s cgiargs: %s\n", is_static, filename, cgiargs);
    if (stat(filename, &sbuf) < 0) {
        printf("404 Not found: Tiny cloudn't find this file\n");
        return;
    }
    if (is_static) { // Serve static content
        if (!(S_ISREG(sbuf.st_mode) || !(S_IRUSR & sbuf.st_mode))) { // S_ISREG -> normal file?, S_IRUSR -> have read permission?
            printf("403 Forbidden: Tiny couldn't read the file\n");
            return;
        }
        serve_static(fd, filename, sbuf.st_size);
    }
    else { // Serve dynamic content
        if (!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode)) {
            printf("403 Forbidden: Tiny couldn't run the CGI program\n");
            return;
        }
        serve_dynamic(fd, filename, cgiargs);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cout << "[main.cpp main][ERROR] Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }
    int listen_fd = open_listen_fd(argv[1]);
    std::cout << "listen_fd: " << listen_fd << std::endl;
    if (listen_fd < 0) {
        return -1;
    }

    while (1) {
        struct sockaddr_storage clientaddr;
        socklen_t client_len = sizeof(clientaddr);
        int conn_fd = accept(listen_fd, (sockaddr *)&clientaddr, &client_len);
        std::cout << "conn_fd: " << conn_fd << std::endl;
        char hostname[MAXLINE], port[MAXLINE];
        getnameinfo((sockaddr *) &clientaddr, client_len, hostname, MAXLINE, port, MAXLINE, 0);
        std::cout << "Accepted connection from " << hostname << ":" << port << std::endl;
        serve_contents(conn_fd);
        // close(connfd);
    }
    return 0;
}
