/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 14:17:30 by hkubo             #+#    #+#             */
/*   Updated: 2023/03/19 16:32:46 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpServer.hpp"
#include "webserv.hpp"

HttpServer::HttpServer() {
    std::cout << "Webserv constructor" << std::endl;
}

HttpServer::HttpServer(int listen_fd) {
    std::cout << "Webserv constructor2" << std::endl;
    set_listen_fd(listen_fd);
}

HttpServer::~HttpServer() { std::cout << "Goodbye, Webserv." << std::endl; }

void HttpServer::set_listen_fd(int listen_fd) { this->listen_fd = listen_fd; }

int HttpServer::get_listen_fd() { return this->listen_fd; }

void HttpServer::run() {
    while (1) {
        struct sockaddr_storage clientaddr;
        socklen_t client_len = sizeof(clientaddr);
        int conn_fd = accept(get_listen_fd(), (sockaddr *)&clientaddr, &client_len);
        std::cout << "conn_fd: " << conn_fd << std::endl;
        char hostname[MAXLINE], port[MAXLINE];
        getnameinfo((sockaddr *)&clientaddr, client_len, hostname, MAXLINE, port, MAXLINE, 0);
        std::cout << "Accepted connection from " << hostname << ":" << port << std::endl;
        serve_contents(conn_fd);
        close(conn_fd);
    }
}
