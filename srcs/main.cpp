/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 22:12:06 by hkubo             #+#    #+#             */
/*   Updated: 2023/03/25 17:41:04 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpServer.hpp"
#include "RequestParser.hpp"
#include "webserv.hpp"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "[ERROR] main: Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }
    int listen_fd = open_listen_fd(argv[1]);
    std::cout << "listen_fd: " << listen_fd << std::endl;
    if (listen_fd < 0) {
        return -1;
    }

    HttpServer server(listen_fd);
    // server.simple_server_run();
    server.multiple_io_server_run();

    return 0;
}
