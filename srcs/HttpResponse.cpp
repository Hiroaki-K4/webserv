/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 14:03:34 by hkubo             #+#    #+#             */
/*   Updated: 2023/03/26 14:31:06 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"

HttpResponse::HttpResponse() {}

HttpResponse::HttpResponse(int conn_fd) { set_conn_fd(conn_fd); }

HttpResponse::~HttpResponse() {}

void HttpResponse::set_conn_fd(int conn_fd) { this->conn_fd = conn_fd; }

int HttpResponse::get_conn_fd() { return this->conn_fd; }

int HttpResponse::parse_uri(char *uri, char *filename, char *cgiargs) {
    char *ptr;

    if (!strstr(uri, "cgi")) {  // Static content
        strcpy(cgiargs, "");
        strcpy(filename, "contents");
        strcat(filename, uri);
        if (uri[strlen(uri) - 1] == '/') strcat(filename, "home.html");
        return 1;
    } else {
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

void HttpResponse::get_filetype(char *filename, char *filetype) {
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

void HttpResponse::serve_static(char *filename, int filesize) {
    int src_fd;
    char *srcp, filetype[MAXLINE];

    // Send response headers to client
    get_filetype(filename, filetype);
    std::stringstream ss;
    ss << "HTTP/1.0, 200 OK\r\nServer: Tiny Web Server\r\nConnection: close\r\nContent-length: " << filesize << "\r\n"
       << "Content-type: " << filetype << "\r\n\r\n";
    std::string out;
    out = ss.str();
    char resp_head[out.length() + 1];
    strcpy(resp_head, out.c_str());
    if (rio_writen(get_conn_fd(), resp_head, strlen(resp_head)) == -1) {
        std::cout << "rio_writen error!" << std::endl;
        return;
    }
    std::cout << "Response headers:" << std::endl;
    std::cout << resp_head;

    // Send response body to client
    src_fd = open(filename, O_RDONLY, 0);
    if (src_fd == -1) {
        std::cout << "[ERROR] serve_static: File open failed." << std::endl;
    }
    srcp = static_cast<char *>(mmap(0, filesize, PROT_READ, MAP_PRIVATE, src_fd, 0));
    close(src_fd);
    rio_writen(get_conn_fd(), srcp, filesize);
    munmap(srcp, filesize);
}

void HttpResponse::serve_dynamic(char *filename, char *cgiargs) {
    char buf[MAXLINE], *emptylist[] = {NULL};

    // Return first part of HTTP response
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    rio_writen(get_conn_fd(), buf, strlen(buf));
    sprintf(buf, "Server: Tiny Web Server\r\n");
    rio_writen(get_conn_fd(), buf, strlen(buf));

    if (fork() == 0) {
        setenv("QUERY_STRING", cgiargs, 1);
        dup2(get_conn_fd(), STDOUT_FILENO);
        execve(filename, emptylist, environ);
    }
    wait(NULL);
}

void HttpResponse::serve_contents() {
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char filename[MAXLINE], cgiargs[MAXLINE];

    rio_t rio;
    rio_readinitb(&rio, get_conn_fd());
    rio_readlineb(&rio, buf, MAXLINE, true);
    std::cout << "Request headers:" << std::endl;
    std::cout << buf;

    RequestParser parser;
    int res = parser.parse_request(buf);
    if (res != EXIT_SUCCESS) {
        return;
    }

    sscanf(buf, "%s %s %s", method, uri, version);
    std::cout << "method: " << parser.get_request_method() << " uri: " << parser.get_target_uri() << " version: " << parser.get_http_version()
              << std::endl;
    int cmp_res = strcasecmp(method, "GET");
    if (cmp_res != 0) {
        std::cout << "Tiny does not implement this method" << std::endl;
        return;
    }

    int is_static = parse_uri(uri, filename, cgiargs);
    std::cout << "is_static: " << is_static << " filename: " << filename << " cgiargs: " << cgiargs << std::endl;
    struct stat sbuf;
    if (stat(filename, &sbuf) < 0) {
        std::cout << "404 Not found: Tiny cloudn't find this file" << std::endl;
        return;
    }
    if (is_static) {
        if (!(S_ISREG(sbuf.st_mode) || !(S_IRUSR & sbuf.st_mode))) {  // S_ISREG -> normal file?, S_IRUSR -> have read permission?
            std::cout << "403 Forbidden: Tiny couldn't read the file" << std::endl;
            return;
        }
        serve_static(filename, sbuf.st_size);
    } else {
        if (!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode)) {
            std::cout << "403 Forbidden: Tiny couldn't run the CGI program" << std::endl;
            return;
        }
        serve_dynamic(filename, cgiargs);
    }
}
