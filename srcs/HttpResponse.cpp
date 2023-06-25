/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 14:03:34 by hkubo             #+#    #+#             */
/*   Updated: 2023/06/24 21:13:11 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"

HttpResponse::HttpResponse() : http_status(200), default_root_dir("contents"), have_location(false), have_request_parser(false) {}

HttpResponse::HttpResponse(int conn_fd, ServerConfig server_config)
    : http_status(200), default_root_dir("contents"), have_location(false), have_request_parser(false) {
    set_conn_fd(conn_fd);
    set_server_config(server_config);
}

HttpResponse::~HttpResponse() {
    if (get_have_location()) {
        delete get_location();
    }
    if (get_have_request_parser()) {
        delete get_request_parser();
    }
}

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

void HttpResponse::set_have_location(bool have_location) { this->have_location = have_location; }

bool HttpResponse::get_have_location() { return this->have_location; }

void HttpResponse::set_have_request_parser(bool have_request_parser) { this->have_request_parser = have_request_parser; }

bool HttpResponse::get_have_request_parser() { return this->have_request_parser; }

bool HttpResponse::check_uri_is_static() {
    if (get_have_location() && get_location()->get_is_cgi()) {
        return false;
    }

    return true;
}

void HttpResponse::set_location(ServerLocation *location) { this->location = location; }

ServerLocation *HttpResponse::get_location() { return this->location; }

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
        std::cout << "[ERROR] create_response_body: File open failed. Failed file name is " << std::string(file_name) << "." << std::endl;
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
    ss << "HTTP/1.1 200 OK\r\nServer: Ultimate Web Server\r\nConnection: close\r\nContent-length: " << file_size << "\r\n"
       << "Content-type: " << filetype << "\r\n\r\n";

    return ss.str();
}

int HttpResponse::serve_static_with_get_method(char *res_head, char *res_body, int file_size) {
    // Send response header
    if (io_write(get_conn_fd(), res_head, strlen(res_head)) == FAILURE) {
        set_http_status(500);
        std::cout << "[ERROR] serve_static: io_write error!" << std::endl;
        return FAILURE;
    }
    std::cout << "Response headers:" << std::endl;
    std::cout << res_head;

    // Send response body
    std::cout << "res_body: " << res_body << std::endl;
    std::cout << "get_conn_fd(): " << get_conn_fd() << std::endl;
    std::cout << "file_size: " << file_size << std::endl;
    if (io_write(get_conn_fd(), res_body, file_size) == FAILURE) {
        set_http_status(500);
        std::cout << "[ERROR] serve_static: io_write error!" << std::endl;
        munmap(res_body, file_size);
        return FAILURE;
    }
    munmap(res_body, file_size);

    return SUCCESS;
}

std::string HttpResponse::get_last_modified_time(std::string target) {
    std::string time;
    struct stat res;
    if (stat(target.c_str(), &res) == 0) {
        time = ctime(&(res.st_mtime));
    }

    return time;
}

std::string HttpResponse::create_page_link(std::string target) {
    std::ostringstream port;
    port << get_server_config().get_port();
    std::cout << "create_page_link_target: " << target << std::endl;
    std::cout << "last: " << get_request_parser()->get_target_uri()[get_request_parser()->get_target_uri().length() - 1] << std::endl;
    std::string link;
    if (get_request_parser()->get_target_uri()[get_request_parser()->get_target_uri().length() - 1] == '/') {
        link = "http://" + get_server_config().get_host_name() + ":" + port.str() + get_request_parser()->get_target_uri() + target;
    } else {
        link = "http://" + get_server_config().get_host_name() + ":" + port.str() + get_request_parser()->get_target_uri() + "/" + target;
    }
    std::cout << "link: " << link << std::endl;

    return link;
}

std::string HttpResponse::create_html_spaces(std::string str, unsigned int align_num) {
    unsigned int spaces_num = align_num - str.length();
    std::stringstream ss;
    for (unsigned int i = 0; i < spaces_num; i++) {
        ss << "&nbsp;";
    }

    return ss.str();
}

int HttpResponse::serve_autoindex() {
    DIR *dir;
    struct dirent *dirent;
    std::string curr_dir = get_default_root_dir() + get_request_parser()->get_target_uri();
    dir = opendir(curr_dir.c_str());
    std::stringstream content;
    if (dir) {
        content << "<html>\r\n<head><title>Index of " << get_request_parser()->get_target_uri() << "</title></head>\r\n<body>\r\n<h1>Index of "
                << get_request_parser()->get_target_uri() << "</h1>\r\n";
        while ((dirent = readdir(dir)) != NULL) {
            std::string target = curr_dir + dirent->d_name;
            std::string modified_time = get_last_modified_time(target);
            if (modified_time[8] == ' ') {
                modified_time.erase(8, 1);
            }
            std::string link = create_page_link(dirent->d_name);
            std::string align_d_name = dirent->d_name;
            unsigned int align_size = 50;
            if (align_d_name.length() > align_size) {
                align_d_name = align_d_name.substr(0, align_size);
            }

            // Create spaces for alignment
            std::string name_spaces = create_html_spaces(align_d_name, align_size);
            std::string modified_time_spaces = create_html_spaces(modified_time, 40);

            if (is_request_uri_dir(target)) {
                content << "<a href=\"" << link << "\">" << align_d_name << "</a> " << name_spaces << modified_time << modified_time_spaces
                        << "-<br>\r\n";
            } else {
                std::ifstream in(target.c_str(), std::ifstream::ate | std::ifstream::binary);
                unsigned int file_size = in.tellg();
                content << "<a href=\"" << link << "\">" << align_d_name << "</a> " << name_spaces << modified_time << modified_time_spaces
                        << file_size << "<br>\r\n";
            }
        }
        content << "</body>\r\n</html>";
        closedir(dir);
    } else {
        std::cout << "[ERROR] HttpResponse::serve_autoindex: Can't open autoindex target folder" << std::endl;
        return FAILURE;
    }

    // Create response header
    char file_type[MAXLINE];
    strcpy(file_type, "text/html");
    std::stringstream res_header;
    res_header << "HTTP/1.1 200 OK\r\nServer: Ultimate Web Server\r\nConnection: close\r\nContent-length: " << content.str().length() << "\r\n"
               << "Content-type: " << file_type << "\r\n\r\n";
    char header[MAXLINE];
    strcpy(header, res_header.str().c_str());

    // Create response body
    char body[content.str().length()];
    strcpy(body, content.str().c_str());

    // Call serve_static_with_get_method
    serve_static_with_get_method(header, body, content.str().length());

    return SUCCESS;
}

int HttpResponse::serve_static(char *file_name, int file_size) {
    if (get_location()->get_allow_method().size() > 0 &&
        std::find(get_location()->get_allow_method().begin(), get_location()->get_allow_method().end(), get_request_parser()->get_request_method()) ==
            get_location()->get_allow_method().end()) {
        std::cout << "[ERROR] HttpResponse::serve_static: Request method is not allowed" << std::endl;
        return FAILURE;
    }

    int src_fd = open(file_name, O_RDONLY, 0);
    if (src_fd == FAILURE) {
        set_http_status(403);
        std::cout << "[ERROR] serve_static: File open failed. Failed file name is " << std::string(file_name) << "." << std::endl;
        return FAILURE;
    }
    close(src_fd);

    // Create response header
    std::string out = create_response_header(file_name, file_size);
    char res_head[out.length() + 1];
    strcpy(res_head, out.c_str());

    // Create response body
    char *res_body = create_response_body(file_name, file_size);
    if (!res_body) {
        munmap(res_body, file_size);
        return FAILURE;
    }

    return serve_static_with_get_method(res_head, res_body, file_size);
}

int HttpResponse::serve_dynamic(char *file_name, char *cgi_args) {
    char buf[MAXLINE], *emptylist[] = {NULL};

    // Return first part of HTTP response
    sprintf(buf, "HTTP/1.1 200 OK\r\n");
    if (io_write(get_conn_fd(), buf, strlen(buf)) == FAILURE) {
        set_http_status(500);
        std::cout << "[ERROR] serve_dynamic: io_write error!" << std::endl;
        return FAILURE;
    }
    sprintf(buf, "Server: Ultimate Web Server\r\n");
    if (io_write(get_conn_fd(), buf, strlen(buf)) == FAILURE) {
        set_http_status(500);
        std::cout << "[ERROR] serve_dynamic: io_write error!" << std::endl;
        return FAILURE;
    }
    if (fork() == 0) {
        setenv("QUERY_STRING", cgi_args, 1);
        dup2(get_conn_fd(), STDOUT_FILENO);
        execve(file_name, emptylist, environ);
    }
    wait(NULL);

    return SUCCESS;
}

void HttpResponse::serve_error_page() {
    std::string error_file_name;
    std::map<int, std::string>::iterator it = get_location()->get_error_pages().find(get_http_status());
    if (it != get_location()->get_error_pages().end()) {
        error_file_name = get_location()->get_root() + get_location()->get_error_pages()[get_http_status()];
    } else {
        error_file_name = get_location()->get_root() + "error.html";
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
    // TODO: Fix error http status
    ss << "HTTP/1.1 200 OK\r\nServer: Ultimate Server\r\nConnection: close\r\nContent-length: " << sbuf.st_size << "\r\n"
       << "Content-type: " << filetype << "\r\n\r\n";
    std::string out;
    out = ss.str();
    char resp_head[out.length() + 1];
    strcpy(resp_head, out.c_str());
    if (io_write(get_conn_fd(), resp_head, strlen(resp_head)) == FAILURE) {
        set_http_status(500);
        std::cout << "[ERROR] serve_error_page: io_write error!" << std::endl;
        return;
    }
    std::cout << "Response error headers:" << std::endl;
    std::cout << resp_head;

    int src_fd = open(file_name, O_RDONLY, 0);
    if (src_fd == FAILURE) {
        set_http_status(500);
        std::cout << "[ERROR] serve_error_page: File open failed. Failed file name is " << std::string(file_name) << "." << std::endl;
        return;
    }

    char *srcp = static_cast<char *>(mmap(0, sbuf.st_size, PROT_READ, MAP_PRIVATE, src_fd, 0));
    close(src_fd);
    io_write(get_conn_fd(), srcp, sbuf.st_size);
    munmap(srcp, sbuf.st_size);
}

RequestParser *HttpResponse::read_http_request() {
    // char buf[MAXLINE];
    io_s *io = new io_s();
    io_init(io, get_conn_fd());
    // io_read_line(io, buf, MAXLINE);
    io_read_request(io);
    // std::cout << "std::string(io->io_buf): " << std::string(io->io_buf) << std::endl;

    RequestParser *parser = new RequestParser(get_server_config().get_client_max_body_size());
    parser->parse_request(std::string(io->io_buf));
    delete io;

    return parser;
}

bool HttpResponse::check_location_info(std::string route, ServerLocation **location) {
    bool have_location = false;

    std::vector<ServerLocation *> locations = get_server_config().get_locations();
    std::vector<std::string> routes;
    for (std::vector<ServerLocation *>::iterator itr = locations.begin(); itr != locations.end(); ++itr) {
        std::string route_with_last_slash;
        std::string route_without_last_slash;
        if ((*itr)->get_route()[(*itr)->get_route().length() - 1] == '/') {
            route_with_last_slash = (*itr)->get_route();
            route_without_last_slash = (*itr)->get_route().substr(0, (*itr)->get_route().length() - 1);
        } else {
            route_with_last_slash = (*itr)->get_route() + "/";
            route_without_last_slash = (*itr)->get_route();
        }

        if (route == route_with_last_slash || route == route_without_last_slash) {
            (*location)->set_route((*itr)->get_route());
            (*location)->set_alias((*itr)->get_alias());
            (*location)->set_root((*itr)->get_root());
            (*location)->set_index((*itr)->get_index());
            (*location)->set_error_pages((*itr)->get_error_pages());
            (*location)->set_allow_method((*itr)->get_allow_method());
            (*location)->set_autoindex((*itr)->get_autoindex());
            (*location)->set_is_cgi((*itr)->get_is_cgi());
            have_location = true;
        }
    }

    return have_location;
}

int HttpResponse::extract_location_info(std::string target_uri, std::string &search_dir) {
    int found = target_uri.length();
    while (found > 0) {
        found = target_uri.find_last_of('/', found);
        if ((size_t)found == std::string::npos) {
            break;
        }
        std::string route = target_uri.substr(0, found + 1);

        ServerLocation *location = new ServerLocation();
        if (check_location_info(route, &location)) {
            search_dir = location->get_root();
            set_default_file(location->get_index());
            set_location(location);
            set_have_location(true);
            return SUCCESS;
        }
        delete location;
        found -= 1;
    }

    return SUCCESS;
}

int HttpResponse::check_http_request(RequestParser *parser) {
    std::string search_dir;
    extract_location_info(parser->get_target_uri(), search_dir);
    if (search_dir != "") {
        set_default_root_dir(search_dir);
    }

    if (parser->get_is_error_request()) {
        set_http_status(parser->get_http_status());
        return FAILURE;
    }

    set_have_request_parser(true);
    set_request_parser(parser);

    set_is_static(check_uri_is_static());

    if (get_have_location() && get_location()->get_autoindex() &&
        is_request_uri_dir(get_default_root_dir() + get_request_parser()->get_target_uri())) {
        return SUCCESS;
    }

    std::string file_name;
    if (get_is_static()) {
        create_static_file_name(parser->get_target_uri(), file_name);
        set_file_name(file_name.c_str());
    } else {
        std::string cgi_args;
        create_dynamic_file_name_and_cgi_args(parser->get_target_uri(), file_name, cgi_args);
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

bool HttpResponse::is_request_uri_dir(std::string uri) {
    struct stat s;
    if (stat(uri.c_str(), &s) == 0) {
        if (s.st_mode & S_IFDIR) {
            return true;
        }
    }

    return false;
}

void HttpResponse::serve_contents() {
    if (get_http_status() >= 400) {
        serve_error_page();
    } else {
        if (get_is_static()) {
            if (get_location()->get_autoindex() && is_request_uri_dir(get_default_root_dir() + get_request_parser()->get_target_uri())) {
                if (serve_autoindex() == FAILURE) {
                    serve_error_page();
                }
            } else {
                if (!(S_ISREG(get_file_info().st_mode) ||
                      !(S_IRUSR & get_file_info().st_mode))) {  // S_ISREG -> normal file?, S_IRUSR -> have read permission?
                    set_http_status(403);
                    std::cout << "[ERROR] serve_contents: Couldn't read the file" << std::endl;
                    serve_error_page();
                }
                if (serve_static(get_file_name(), get_file_info().st_size) == FAILURE) {
                    serve_error_page();
                }
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
