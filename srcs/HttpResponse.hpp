/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 14:03:45 by hkubo             #+#    #+#             */
/*   Updated: 2023/05/05 18:06:27 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <algorithm>

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
    void set_location(const ServerLocation location);
    ServerLocation get_location();
    void set_request_parser(RequestParser *request_parser);
    RequestParser *get_request_parser();

    int create_static_file_name(std::string uri, std::string &file_name);
    int create_dynamic_file_name_and_cgi_args(std::string uri, std::string &file_name, std::string &cgi_args);
    bool check_uri_is_static(const std::string uri);
    void get_filetype(char *file_name, char *filetype);
    int serve_static_with_get_method(char *file_name, int file_size);
    int serve_static(char *file_name, int file_size);
    int serve_dynamic(char *file_name, char *cgi_args);
    void serve_error_page();
    RequestParser read_http_request();
    bool check_location_info(std::string route, ServerLocation **location);
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
    ServerLocation location;
    RequestParser *request_parser;
};

#endif
