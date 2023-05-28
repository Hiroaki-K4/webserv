/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 14:17:30 by hkubo             #+#    #+#             */
/*   Updated: 2023/05/28 20:39:21 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpServer.hpp"

#include "io_utils.hpp"

HttpServer::HttpServer() {}

HttpServer::HttpServer(int listen_fd) { set_listen_fd(listen_fd); }

HttpServer::~HttpServer() { std::cout << "Goodbye, Webserv." << std::endl; }

void HttpServer::set_listen_fd(const int listen_fd) { this->listen_fd = listen_fd; }

int HttpServer::get_listen_fd() { return this->listen_fd; }

void HttpServer::multiple_io_server_run(ServerConfig server_config) {
    fd_set read_set, ready_set;

    FD_ZERO(&read_set);
    FD_SET(get_listen_fd(), &read_set);

    while (true) {
        ready_set = read_set;
        select(get_listen_fd() + 1, &ready_set, NULL, NULL, NULL);
        if (FD_ISSET(get_listen_fd(), &ready_set)) {
            struct sockaddr_storage clientaddr;
            socklen_t client_len = sizeof(clientaddr);
            int conn_fd = accept(get_listen_fd(), (sockaddr *)&clientaddr, &client_len);
            std::cout << "conn_fd: " << conn_fd << std::endl;
            char host_name[MAXLINE], port[MAXLINE];
            getnameinfo((sockaddr *)&clientaddr, client_len, host_name, MAXLINE, port, MAXLINE, 0);
            std::cout << "Accepted connection from " << host_name << ":" << port << std::endl;
            HttpResponse resp(conn_fd, server_config);
            resp.check_http_request(resp.read_http_request());
            resp.serve_contents();
            close(conn_fd);
        }
    }
}
