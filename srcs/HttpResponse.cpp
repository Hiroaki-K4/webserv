/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 14:03:34 by hkubo             #+#    #+#             */
/*   Updated: 2023/04/22 16:01:46 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"

HttpResponse::HttpResponse() : http_status(200) {}

HttpResponse::HttpResponse(int conn_fd, ServerConfig server_config) : http_status(200) {
    set_conn_fd(conn_fd);
    set_server_config(server_config);
}

HttpResponse::~HttpResponse() {}

void HttpResponse::set_conn_fd(const int conn_fd) { this->conn_fd = conn_fd; }

int HttpResponse::get_conn_fd() { return this->conn_fd; }

void HttpResponse::set_http_status(const unsigned int http_status) { this->http_status = http_status; }

unsigned int HttpResponse::get_http_status() { return this->http_status; };

void HttpResponse::set_is_static(bool is_static) { this->is_static = is_static; }

bool HttpResponse::get_is_static() { return this->is_static; }

void HttpResponse::set_file_name(const char *file_name) { strcpy(this->file_name, file_name); }

char *HttpResponse::get_file_name() { return this->file_name; }

void HttpResponse::set_cgi_args(const char *cgi_args) { strcpy(this->cgi_args, cgi_args); }

char *HttpResponse::get_cgi_args() { return this->cgi_args; }

void HttpResponse::set_file_info(const struct stat file_info) { this->file_info = file_info; }

struct stat HttpResponse::get_file_info() { return this->file_info; }

void HttpResponse::set_server_config(const ServerConfig server_config) { this->server_config = server_config; }

ServerConfig HttpResponse::get_server_config() { return this->server_config; }

bool HttpResponse::parse_uri(char *uri, char *file_name, char *cgi_args) {
    char *ptr;

    if (!strstr(uri, "cgi")) {  // Static content
        strcpy(cgi_args, "");
        strcpy(file_name, "contents");
        strcat(file_name, uri);
        if (uri[strlen(uri) - 1] == '/') strcat(file_name, "home.html");
        return true;
    } else {
        ptr = index(uri, '?');
        if (ptr) {
            strcpy(cgi_args, ptr + 1);
            *ptr = '\0';  // Change ? to null terminator
        } else {
            strcpy(cgi_args, "");
        }
        strcpy(file_name, "./contents");
        strcat(file_name, uri);
        return false;
    }
}

void HttpResponse::get_filetype(char *file_name, char *filetype) {
    if (strstr(file_name, ".html"))
        strcpy(filetype, "text/html");
    else if (strstr(file_name, ".gif"))
        strcpy(filetype, "image/gif");
    else if (strstr(file_name, ".png"))
        strcpy(filetype, "image/png");
    else if (strstr(file_name, ".jpg"))
        strcpy(filetype, "image/jpeg");
    else
        strcpy(filetype, "text/plain");
}

int HttpResponse::serve_static(char *file_name, int filesize) {
    // Send response body to client
    int src_fd = open(file_name, O_RDONLY, 0);
    if (src_fd == FAILURE) {
        set_http_status(403);
        std::cout << "[ERROR] serve_static: File open failed." << std::endl;
        return FAILURE;
    }
    char *srcp = static_cast<char *>(mmap(0, filesize, PROT_READ, MAP_PRIVATE, src_fd, 0));
    close(src_fd);

    char filetype[MAXLINE];
    // Send response headers to client
    get_filetype(file_name, filetype);
    std::stringstream ss;
    ss << "HTTP/1.0, 200 OK\r\nServer: Ultimate Web Server\r\nConnection: close\r\nContent-length: " << filesize << "\r\n"
       << "Content-type: " << filetype << "\r\n\r\n";
    std::string out;
    out = ss.str();
    char resp_head[out.length() + 1];
    strcpy(resp_head, out.c_str());
    if (rio_writen(get_conn_fd(), resp_head, strlen(resp_head)) == FAILURE) {
        set_http_status(500);
        std::cout << "[ERROR] serve_static: rio_writen error!" << std::endl;
        return FAILURE;
    }
    std::cout << "Response headers:" << std::endl;
    std::cout << resp_head;
    if (rio_writen(get_conn_fd(), srcp, filesize) == FAILURE) {
        set_http_status(500);
        std::cout << "[ERROR] serve_static: rio_writen error!" << std::endl;
        munmap(srcp, filesize);
        return FAILURE;
    }
    munmap(srcp, filesize);

    return SUCCESS;
}

int HttpResponse::serve_dynamic(char *file_name, char *cgi_args) {
    char buf[MAXLINE], *emptylist[] = {NULL};

    // Return first part of HTTP response
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    if (rio_writen(get_conn_fd(), buf, strlen(buf)) == FAILURE) {
        set_http_status(500);
        std::cout << "[ERROR] serve_dynamic: rio_writen error!" << std::endl;
        return FAILURE;
    }
    sprintf(buf, "Server: Ultimate Web Server\r\n");
    if (rio_writen(get_conn_fd(), buf, strlen(buf)) == FAILURE) {
        set_http_status(500);
        std::cout << "[ERROR] serve_dynamic: rio_writen error!" << std::endl;
        return FAILURE;
    }
    if (fork() == 0) {
        setenv("QUERY_STRING", cgi_args, 1);
        dup2(get_conn_fd(), STDOUT_FILENO);
        execve(file_name, emptylist, environ);
    }
    wait(NULL);

    return FAILURE;
}

void HttpResponse::serve_error_page() {
    char file_name[MAXLINE] = "contents/error.html";
    struct stat sbuf;
    if (stat(file_name, &sbuf) < 0) {
        set_http_status(500);
        std::cout << "[ERROR] serve_error_page: Cloudn't find this file" << std::endl;
        return;
    }

    char filetype[MAXLINE];
    get_filetype(file_name, filetype);
    std::stringstream ss;
    ss << "HTTP/1.0, 200 OK\r\nServer: Ultimate Server\r\nConnection: close\r\nContent-length: " << sbuf.st_size << "\r\n"
       << "Content-type: " << filetype << "\r\n\r\n";
    std::string out;
    out = ss.str();
    char resp_head[out.length() + 1];
    strcpy(resp_head, out.c_str());
    if (rio_writen(get_conn_fd(), resp_head, strlen(resp_head)) == FAILURE) {
        set_http_status(500);
        std::cout << "[ERROR] serve_error_page: rio_writen error!" << std::endl;
        return;
    }
    std::cout << "Response headers:" << std::endl;
    std::cout << resp_head;

    int src_fd = open(file_name, O_RDONLY, 0);
    if (src_fd == FAILURE) {
        set_http_status(500);
        std::cout << "[ERROR] serve_error_page: File open failed." << std::endl;
        return;
    }
    char *srcp;
    srcp = static_cast<char *>(mmap(0, sbuf.st_size, PROT_READ, MAP_PRIVATE, src_fd, 0));
    std::cout << "srcp: " << srcp << std::endl;
    close(src_fd);
    rio_writen(get_conn_fd(), srcp, sbuf.st_size);
    munmap(srcp, sbuf.st_size);
}

RequestParser HttpResponse::read_http_request() {
    char buf[MAXLINE];
    rio_t rio;
    rio_readinitb(&rio, get_conn_fd());
    rio_readlineb(&rio, buf, MAXLINE, true);
    std::cout << "Request headers:" << std::endl;
    std::cout << buf;

    RequestParser parser;
    parser.parse_request(buf);

    return parser;
}

void HttpResponse::check_http_request(RequestParser parser) {
    if (parser.get_is_error_request()) {
        set_http_status(parser.get_http_status());
        return;
    }

    char file_name[MAXLINE], cgi_args[MAXLINE];
    char target_uri[parser.get_target_uri().length() + 1];
    strcpy(target_uri, parser.get_target_uri().c_str());
    bool is_static = parse_uri(target_uri, file_name, cgi_args);
    set_is_static(is_static);
    set_file_name(file_name);
    set_cgi_args(cgi_args);
    std::cout << "is_static: " << get_is_static() << " file_name: " << get_file_name() << " cgi_args: " << get_cgi_args() << std::endl;
    struct stat file_info;
    if (stat(file_name, &file_info) < 0) {
        set_http_status(404);
        std::cout << "[ERROR] check_http_request: Cloudn't find this file" << std::endl;
        return;
    }
    set_file_info(file_info);
}

void HttpResponse::serve_contents() {
    if (get_http_status() >= 400) {
        serve_error_page();
    } else {
        if (get_is_static()) {
            if (!(S_ISREG(get_file_info().st_mode) ||
                  !(S_IRUSR & get_file_info().st_mode))) {  // S_ISREG -> normal file?, S_IRUSR -> have read permission?
                set_http_status(403);
                std::cout << "[ERROR] serve_contents: Couldn't read the file" << std::endl;
                serve_error_page();
            }
            if (serve_static(get_file_name(), get_file_info().st_size) == FAILURE) {
                serve_error_page();
            }
        } else {
            if (!(S_ISREG(get_file_info().st_mode)) || !(S_IXUSR & get_file_info().st_mode)) {
                set_http_status(403);
                std::cout << "[ERROR] serve_contents: Couldn't run the CGI program" << std::endl;
                serve_error_page();
            }
            if (serve_dynamic(get_file_name(), get_cgi_args()) == FAILURE) {
                serve_error_page();
            }
        }
    }
}
