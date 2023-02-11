/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 22:22:17 by hkubo             #+#    #+#             */
/*   Updated: 2023/02/11 17:46:11 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
        if (connect(client_fd, p->ai_addr, p->ai_addrlen) != -1) break;
        close(client_fd);
    }
    freeaddrinfo(listp);
    if (!p)
        return -1;
    else
        return client_fd;
}

int main(int argc, char *argv[]) {
    char buf[MAXLINE];
    rio_t rio;

    if (argc != 3) {
        std::cout << "[ERROR] Usage: " << argv[0] << " <host> <port>" << std::endl;
        return 1;
    }

    int client_fd = open_client_fd(argv[1], argv[2]);
    std::cout << "client_fd: " << client_fd << std::endl;

    rio_readinitb(&rio, client_fd);

    while (fgets(buf, MAXLINE, stdin) != NULL) {
        // Write the string received in standard input to clientfd.
        if (rio_writen(client_fd, buf, strlen(buf)) == -1) {
            std::cout << "rio_writen error" << std::endl;
            return (-1);
        }
        rio_readlineb(&rio, buf, MAXLINE, false);
        fputs(buf, stdout);
    }
    close(client_fd);
    return 0;
}
