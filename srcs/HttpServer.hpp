/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 14:17:33 by hkubo             #+#    #+#             */
/*   Updated: 2023/04/22 16:00:37 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

#include <netdb.h>
#include <sys/select.h>
#include <unistd.h>

#include <iostream>

#include "HttpResponse.hpp"
#include "ServerConfig.hpp"

#define MAXLINE 8192

class HttpServer {
   public:
    HttpServer();
    HttpServer(int listen_fd);
    ~HttpServer();

    void set_listen_fd(const int listen_fd);
    int get_listen_fd();
    void multiple_io_server_run(ServerConfig server);

   private:
    int listen_fd;
};

#endif
