/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 14:03:34 by hkubo             #+#    #+#             */
/*   Updated: 2023/05/13 18:16:44 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"

HttpResponse::HttpResponse() : http_status(200), default_root_dir("contents") {}

HttpResponse::HttpResponse(int conn_fd, ServerConfig server_config) : http_status(200), default_root_dir("contents") {
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

void HttpResponse::set_default_root_dir(const std::string default_root_dir) { this->default_root_dir = default_root_dir; }

std::string HttpResponse::get_default_root_dir() { return this->default_root_dir; }

void HttpResponse::set_default_file(const std::string default_file) { this->default_file = default_file; }

std::string HttpResponse::get_default_file() { return this->default_file; }

char *HttpResponse::get_cgi_args() { return this->cgi_args; }

void HttpResponse::set_file_info(const struct stat file_info) { this->file_info = file_info; }

struct stat HttpResponse::get_file_info() {
    return this->file_info;
}

void HttpResponse::set_server_config(const ServerConfig server_config) { this->server_config = server_config; }

ServerConfig HttpResponse::get_server_config() { return this->server_config; }

void HttpResponse::set_request_parser(RequestParser *request_parser) { this->request_parser = request_parser; }

RequestParser *HttpResponse::get_request_parser() { return this->request_parser; }

bool HttpResponse::check_uri_is_static(const std::string uri) {
    if (uri.find("cgi") == std::string::npos) {
        return true;
    }
    return false;
}

void HttpResponse::set_location(const ServerLocation location) { this->location = location; }

ServerLocation HttpResponse::get_location() { return this->location; }

int HttpResponse::create_static_file_name(std::string uri, std::string &file_name) {
    file_name = get_default_root_dir() + uri;
    if (uri[uri.length() - 1] == '/') {
        file_name = file_name + get_default_file();
    }

    return SUCCESS;
}

int HttpResponse::create_dynamic_file_name_and_cgi_args(std::string uri, std::string &file_name, std::string &cgi_args) {
    std::size_t found = uri.find('?');
    if (found != std::string::npos) {
        cgi_args = uri.substr(found + 1);
        uri = uri.substr(0, found);
    }

    file_name = "./contents" + uri;

    return SUCCESS;
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

char *HttpResponse::create_response_body(char *file_name, int file_size) {
    int src_fd = open(file_name, O_RDONLY, 0);
    if (src_fd == FAILURE) {
        set_http_status(403);
        std::cout << "[ERROR] serve_static: File open failed." << std::endl;
        return NULL;
    }
    char *res_body = static_cast<char *>(mmap(0, file_size, PROT_READ, MAP_PRIVATE, src_fd, 0));
    close(src_fd);

    return res_body;
}

std::string HttpResponse::create_response_header(char *file_name, int file_size) {
    char filetype[MAXLINE];
    get_filetype(file_name, filetype);
    std::stringstream ss;
    ss << "HTTP/1.0, 200 OK\r\nServer: Ultimate Web Server\r\nConnection: close\r\nContent-length: " << file_size << "\r\n"
       << "Content-type: " << filetype << "\r\n\r\n";

    return ss.str();
}

int HttpResponse::serve_static_with_get_method(char *res_head, char *res_body, int file_size) {
    // Send response header
    if (rio_writen(get_conn_fd(), res_head, strlen(res_head)) == FAILURE) {
        set_http_status(500);
        std::cout << "[ERROR] serve_static: rio_writen error!" << std::endl;
        return FAILURE;
    }
    std::cout << "Response headers:" << std::endl;
    std::cout << res_head;

    // Send response body
    if (rio_writen(get_conn_fd(), res_body, file_size) == FAILURE) {
        set_http_status(500);
        std::cout << "[ERROR] serve_static: rio_writen error!" << std::endl;
        munmap(res_body, file_size);
        return FAILURE;
    }
    munmap(res_body, file_size);

    return SUCCESS;
}

int HttpResponse::serve_static(char *file_name, int file_size) {
    if (get_location().get_allow_method().size() > 0 &&
        std::find(get_location().get_allow_method().begin(), get_location().get_allow_method().end(), get_request_parser()->get_request_method()) ==
            get_location().get_allow_method().end()) {
        std::cout << "[ERROR] HttpResponse::serve_static: Request method is not allowed" << std::endl;
        return FAILURE;
    }

    // TODO: Add POST and DELETE
    // if (get_request_parser()->get_request_method() == "GET") {
    //     return serve_static_with_get_method(file_name, file_size);
    // }

    int src_fd = open(file_name, O_RDONLY, 0);
    if (src_fd == FAILURE) {
        set_http_status(403);
        std::cout << "[ERROR] serve_static: File open failed." << std::endl;
        return FAILURE;
    }
    close(src_fd);

    // Create response body
    char *res_body = create_response_body(file_name, file_size);
    if (!res_body) {
        munmap(res_body, file_size);
        return FAILURE;
    }

    // Create response header
    std::string out = create_response_header(file_name, file_size);
    char res_head[out.length() + 1];
    strcpy(res_head, out.c_str());

    return serve_static_with_get_method(res_head, res_body, file_size);
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
    std::string error_file_name;
    if (get_location().get_error_pages().find(get_http_status()) != get_location().get_error_pages().end()) {
        error_file_name = get_location().get_root() + get_location().get_error_pages()[get_http_status()];
    } else {
        error_file_name = get_location().get_root() + "error.html";
    }
    char file_name[MAXLINE];
    strcpy(file_name, error_file_name.c_str());

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

    RequestParser parser(get_server_config().get_client_max_body_size());
    parser.parse_request(buf);

    return parser;
}

bool HttpResponse::check_location_info(std::string route, ServerLocation **location) {
    bool have_location = false;

    std::vector<ServerLocation *> locations = get_server_config().get_locations();
    std::vector<std::string> routes;
    for (std::vector<ServerLocation *>::iterator itr = locations.begin(); itr != locations.end(); ++itr) {
        if (route == (*itr)->get_route()) {
            *location = (*itr);
            have_location = true;
        }
    }

    return have_location;
}

int HttpResponse::extract_location_info(std::string target_uri, std::string &search_dir) {
    size_t found = 0;
    while (found < target_uri.length()) {
        found = target_uri.find('/', found);
        if (found == std::string::npos) {
            break;
        }
        std::string route = target_uri.substr(0, found + 1);

        ServerLocation *location = new ServerLocation();
        if (check_location_info(route, &location)) {
            search_dir = location->get_root();
            set_default_file(location->get_index());
            set_location(*location);
            delete location;
            return SUCCESS;
        }
        delete location;
        found += 1;
    }

    return SUCCESS;
}

int HttpResponse::check_http_request(RequestParser parser) {
    if (parser.get_is_error_request()) {
        set_http_status(parser.get_http_status());
        return FAILURE;
    }

    set_request_parser(&parser);
    set_is_static(check_uri_is_static(parser.get_target_uri()));

    std::string file_name;
    std::string search_dir;
    extract_location_info(parser.get_target_uri(), search_dir);
    if (search_dir != "") {
        set_default_root_dir(search_dir);
    }
    if (get_is_static()) {
        create_static_file_name(parser.get_target_uri(), file_name);
        set_file_name(file_name.c_str());
    } else {
        std::string cgi_args;
        create_dynamic_file_name_and_cgi_args(parser.get_target_uri(), file_name, cgi_args);
        set_file_name(file_name.c_str());
        set_cgi_args(cgi_args.c_str());
    }

    std::cout << "is_static: " << get_is_static() << " file_name: " << get_file_name() << " cgi_args: " << get_cgi_args() << std::endl;

    struct stat file_info;
    if (stat(file_name.c_str(), &file_info) < 0) {
        set_http_status(404);
        std::cout << "[ERROR] check_http_request: Cloudn't find this file" << std::endl;
        return FAILURE;
    }
    set_file_info(file_info);

    return SUCCESS;
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
