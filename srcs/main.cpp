/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 22:12:06 by hkubo             #+#    #+#             */
/*   Updated: 2023/05/04 17:03:34 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"
#include "HttpServer.hpp"
#include "RequestParser.hpp"
#include "webserv.hpp"

void *server_thread(void *arg) {
    ServerConfig *server = (ServerConfig *)arg;

    std::stringstream ss;
    ss << server->get_port();
    char host_name[MAXLINE], port[MAXLINE];
    strcpy(host_name, server->get_host_name().c_str());
    strcpy(port, ss.str().c_str());
    int listen_fd;
    if (server->get_host_name() == "") {
        listen_fd = open_listen_fd(NULL, port);
    } else {
        listen_fd = open_listen_fd(host_name, port);
    }
    std::cout << "[INFO] main: listen fd is " << listen_fd << std::endl;
    if (listen_fd < 0) {
        return NULL;
    }

    HttpServer http_server(listen_fd);
    http_server.multiple_io_server_run(*(server));

    return NULL;
}

void exit_thread(pthread_t *thread, int thread_num) {
    for (int i = 0; i < thread_num; i++) {
        pthread_join(thread[i], NULL);
    }
    free(thread);
}

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

    pthread_t *thread = (pthread_t *)malloc(sizeof(pthread_t) * servers.size());
    if (!thread) {
        std::cout << "[ERROR] main: thread allocation failed" << std::endl;
        return EXIT_FAILURE;
    }

    for (unsigned int i = 0; i < servers.size(); i++) {
        if (pthread_create(&thread[i], NULL, server_thread, servers[i]) != 0) {
            std::cout << "[ERROR] main: pthread_create failed" << std::endl;
            return EXIT_FAILURE;
        }
    }

    exit_thread(thread, servers.size());

    return EXIT_FAILURE;
}
