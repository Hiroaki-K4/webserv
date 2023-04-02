/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 22:22:17 by hkubo             #+#    #+#             */
/*   Updated: 2023/04/02 20:53:52 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <thread>

#include "webserv.hpp"

int open_client_fd(char *hostname, char *port) {
    int client_fd;
    struct addrinfo hints, *listp, *p;

    std::memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV;
    hints.ai_flags |= AI_ADDRCONFIG;
    getaddrinfo(hostname, port, &hints, &listp);

    for (p = listp; p; p = p->ai_next) {
        if ((client_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) continue;
        if (connect(client_fd, p->ai_addr, p->ai_addrlen) != FAIL) break;
        close(client_fd);
    }
    freeaddrinfo(listp);
    if (!p)
        return FAIL;
    else
        return client_fd;
}

void call_server(char *argv[], std::string str) {
    // std::string str = "GET / HTTP/1.1\n";
    char *buf = new char[str.length()];
    strcpy(buf, str.c_str());
    int client_fd = open_client_fd(argv[1], argv[2]);
    std::cout << "client_fd: " << client_fd << std::endl;

    rio_t rio;
    rio_readinitb(&rio, client_fd);
    if (rio_writen(client_fd, buf, strlen(buf)) == FAIL) {
        std::cout << "rio_writen error" << std::endl;
        return;
    }

    rio_readlineb(&rio, buf, MAXLINE, false);
    std::cout << buf << std::endl;
    close(client_fd);
    delete buf;
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        std::cout << "[ERROR] Usage: " << argv[0] << " <host> <port> <use_stdin> <repeat_times>" << std::endl;
        return 1;
    }

    int client_fd;
    std::string use_stdin(argv[3]);
    if (use_stdin == "1") {
        char buf[MAXLINE];
        client_fd = open_client_fd(argv[1], argv[2]);
        std::cout << "client_fd: " << client_fd << std::endl;
        rio_t rio;
        rio_readinitb(&rio, client_fd);

        if (fgets(buf, MAXLINE, stdin) != NULL) {
            // Write the string received in standard input to clientfd.
            if (rio_writen(client_fd, buf, strlen(buf)) == FAIL) {
                std::cout << "rio_writen error" << std::endl;
                return FAIL;
            }
            rio_readlineb(&rio, buf, MAXLINE, false);
            fputs(buf, stdout);
        }
        close(client_fd);
    } else {
        std::string str = "GET / HTTP/1.1\n";
        int loop_num = atoi(argv[4]);
        for (int i = 0; i < loop_num; i++) {
            call_server(argv, str);
            // sleep(1);
        }
    }

    return 0;
}
