/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 14:03:45 by hkubo             #+#    #+#             */
/*   Updated: 2023/03/26 14:56:37 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include "webserv.hpp"
#include "RequestParser.hpp"

class HttpResponse {
   public:
    HttpResponse();
    HttpResponse(int conn_fd);
    ~HttpResponse();

    void set_conn_fd(int conn_fd);
    int get_conn_fd();
    void set_http_status(unsigned int http_status);
    unsigned int get_http_status();

    int parse_uri(char *uri, char *filename, char *cgiargs);
    void get_filetype(char *filename, char *filetype);
    void serve_static(char *filename, int filesize);
    void serve_dynamic(char *filename, char *cgiargs);
    void serve_error_page();
    void serve_contents();


   private:
    int conn_fd;
    unsigned int http_status;
};

#endif
