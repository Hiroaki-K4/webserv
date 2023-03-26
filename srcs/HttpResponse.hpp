/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 14:03:45 by hkubo             #+#    #+#             */
/*   Updated: 2023/03/26 17:28:03 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include "RequestParser.hpp"
#include "webserv.hpp"

class HttpResponse {
   public:
    HttpResponse();
    HttpResponse(int conn_fd);
    ~HttpResponse();

    void set_conn_fd(int conn_fd);
    int get_conn_fd();
    void set_http_status(unsigned int http_status);
    unsigned int get_http_status();
    void set_is_static(bool is_static);
    bool get_is_static();
    void set_file_name(char *file_name);
    char *get_file_name();
    void set_cgi_args(char *cgi_args);
    char *get_cgi_args();
    void set_file_info(struct stat file_info);
    struct stat get_file_info();

    bool parse_uri(char *uri, char *file_name, char *cgi_args);
    void get_filetype(char *file_name, char *filetype);
    void serve_static(char *file_name, int filesize);
    void serve_dynamic(char *file_name, char *cgi_args);
    void serve_error_page();
    void check_http_request();
    void serve_contents();


   private:
    int conn_fd;
    bool is_static;
    unsigned int http_status;

    // request info
    char file_name[MAXLINE];
    char cgi_args[MAXLINE];
    struct stat file_info;
};

#endif
