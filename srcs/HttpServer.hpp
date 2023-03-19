/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 14:17:33 by hkubo             #+#    #+#             */
/*   Updated: 2023/03/19 16:37:07 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

#include <iostream>
#include <netdb.h>
#include <unistd.h>

#define MAXLINE 8192

class HttpServer {
   public:
    HttpServer();
    HttpServer(int listen_fd);
    ~HttpServer();

    void set_listen_fd(int listen_fd);
    int get_listen_fd();
    void run();

   private:
    int listen_fd;
};

#endif
