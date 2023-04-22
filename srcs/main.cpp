/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 22:12:06 by hkubo             #+#    #+#             */
/*   Updated: 2023/04/22 15:30:34 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"
#include "HttpServer.hpp"
#include "RequestParser.hpp"
#include "webserv.hpp"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "[ERROR] main: Usage: " << argv[0] << " <config file>" << std::endl;
        return EXIT_FAILURE;
    }

    ConfigParser config_parser;
    if (config_parser.parse_config(argv[1]) == FAILURE) {
        return EXIT_FAILURE;
    }

    std::vector<ServerConfig *> servers = config_parser.get_servers();
    std::stringstream ss;
    ss << servers[0]->get_port();
    char port[MAXLINE];
    strcpy(port, ss.str().c_str());
    int listen_fd = open_listen_fd(port);
    std::cout << "listen_fd: " << listen_fd << std::endl;
    if (listen_fd < 0) {
        return EXIT_FAILURE;
    }

    HttpServer server(listen_fd);
    server.multiple_io_server_run();

    return EXIT_FAILURE;
}
