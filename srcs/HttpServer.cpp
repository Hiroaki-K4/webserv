/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 14:17:30 by hkubo             #+#    #+#             */
/*   Updated: 2023/03/26 14:28:43 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpServer.hpp"

#include "webserv.hpp"

HttpServer::HttpServer() {}

HttpServer::HttpServer(int listen_fd) { set_listen_fd(listen_fd); }

HttpServer::~HttpServer() { std::cout << "Goodbye, Webserv." << std::endl; }

void HttpServer::set_listen_fd(int listen_fd) { this->listen_fd = listen_fd; }

int HttpServer::get_listen_fd() { return this->listen_fd; }

void HttpServer::simple_server_run() {
    while (1) {
        struct sockaddr_storage clientaddr;
        socklen_t client_len = sizeof(clientaddr);
        int conn_fd = accept(get_listen_fd(), (sockaddr *)&clientaddr, &client_len);
        std::cout << "conn_fd: " << conn_fd << std::endl;
        char host_name[MAXLINE], port[MAXLINE];
        getnameinfo((sockaddr *)&clientaddr, client_len, host_name, MAXLINE, port, MAXLINE, 0);
        std::cout << "Accepted connection from " << host_name << ":" << port << std::endl;
        HttpResponse resp(conn_fd);
        resp.serve_contents();
        close(conn_fd);
    }
}

void HttpServer::multiple_io_server_run() {
    fd_set read_set, ready_set;

    FD_ZERO(&read_set);
    FD_SET(get_listen_fd(), &read_set);

    while (1) {
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
            HttpResponse resp(conn_fd);
            resp.serve_contents();
            close(conn_fd);
        }
    }
}
