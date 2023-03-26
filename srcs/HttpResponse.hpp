/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 14:03:45 by hkubo             #+#    #+#             */
/*   Updated: 2023/03/26 14:30:21 by hkubo            ###   ########.fr       */
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

    int parse_uri(char *uri, char *filename, char *cgiargs);
    void get_filetype(char *filename, char *filetype);
    void serve_static(char *filename, int filesize);
    void serve_dynamic(char *filename, char *cgiargs);
    void serve_contents();


   private:
    int conn_fd;
};

#endif
