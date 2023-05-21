/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 14:03:45 by hkubo             #+#    #+#             */
/*   Updated: 2023/05/21 17:34:36 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <dirent.h>

#include <algorithm>
#include <fstream>
#include <iomanip>

#include "RequestParser.hpp"
#include "ServerConfig.hpp"
#include "ServerLocation.hpp"
#include "webserv.hpp"

class HttpResponse {
   public:
    HttpResponse();
    HttpResponse(int conn_fd, ServerConfig server_config);
    ~HttpResponse();

    void set_conn_fd(const int conn_fd);
    int get_conn_fd();
    void set_http_status(unsigned int http_status);
    unsigned int get_http_status();
    void set_is_static(bool is_static);
    bool get_is_static();
    void set_file_name(const char *file_name);
    char *get_file_name();
    void set_cgi_args(const char *cgi_args);
    char *get_cgi_args();
    void set_default_root_dir(const std::string default_root_dir);
    std::string get_default_root_dir();
    void set_default_file(const std::string default_file);
    std::string get_default_file();
    void set_file_info(const struct stat file_info);
    struct stat get_file_info();
    void set_server_config(const ServerConfig server_config);
    ServerConfig get_server_config();
    void set_location(ServerLocation *location);
    ServerLocation *get_location();
    void set_request_parser(RequestParser *request_parser);
    RequestParser *get_request_parser();
    void set_have_location(bool have_location);
    bool get_have_location();

    int create_static_file_name(std::string uri, std::string &file_name);
    int create_dynamic_file_name_and_cgi_args(std::string uri, std::string &file_name, std::string &cgi_args);
    bool check_uri_is_static();
    void get_filetype(char *file_name, char *filetype);
    char *create_response_body(char *file_name, int file_size);
    std::string create_response_header(char *file_name, int file_size);
    int serve_static_with_get_method(char *res_head, char *res_body, int file_size);
    int serve_autoindex();
    int serve_static(char *file_name, int file_size);
    int serve_dynamic(char *file_name, char *cgi_args);
    void serve_error_page();
    RequestParser read_http_request();
    bool check_location_info(std::string route, ServerLocation **location);
    bool is_request_uri_dir(std::string uri);
    std::string get_last_modified_time(std::string target);
    std::string create_page_link(std::string target);
    std::string create_html_spaces(std::string str, unsigned int align_num);
    int extract_location_info(std::string target_uri, std::string &search_dir);
    int check_http_request(RequestParser parser);
    void serve_contents();

   private:
    int conn_fd;
    bool is_static;
    unsigned int http_status;
    char file_name[MAXLINE];
    char cgi_args[MAXLINE];
    std::string default_root_dir;
    std::string default_file;
    struct stat file_info;
    ServerConfig server_config;
    bool have_location;
    ServerLocation *location;
    RequestParser *request_parser;
};

#endif
