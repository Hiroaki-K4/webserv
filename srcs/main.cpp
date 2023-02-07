/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 22:12:06 by hkubo             #+#    #+#             */
/*   Updated: 2023/02/07 23:35:56 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int open_listen_fd(char *port)
{
    int listen_fd, opt_val = 1;
    char buf[20];
    struct addrinfo hints, *listp, *p;

    std::memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM; // Ordered and reliable.
    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG; // AI_PASSIVE->server
    hints.ai_flags = AI_NUMERICSERV; // port should be string include port number.
    getaddrinfo(NULL, port, &hints, &listp);

    for (p = listp; p; p = p->ai_next) {
        if ((listen_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
            continue;
        setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt_val, sizeof(int));

        // bind function ask the kernel to relate server socket address and socket descriptor(listen_fd).
        if (bind(listen_fd, p->ai_addr, p->ai_addrlen) == 0)
        {
            getnameinfo(p->ai_addr, p->ai_addrlen, buf, 20, NULL, 0, NI_NUMERICHOST);
            std::cout << "Host address: " << buf << std::endl;
            break;
        }
        close(listen_fd);
    }

    freeaddrinfo(listp);
    if (!p) {
        std::cout << "[main.cpp open_listen_fd][ERROR] freeaddrinfo error" << std::endl;
        return -1;
    }
    // Change listen_Fd from active socket to listen socket(wait connection request from client).
    if (listen(listen_fd, LISTENQ) < 0) {
        std::cout << "[main.cpp open_listen_fd][ERROR] listen error" << std::endl;
        close(listen_fd);
        return -1;
    }

    return listen_fd;
}

int main(int argc, char *argv[])
{
    int listen_fd;

    if (argc != 2) {
        std::cout << "[main.cpp main][ERROR] Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }
    listen_fd = open_listen_fd(argv[1]);
    std::cout << "listen_fd: " << listen_fd << std::endl;
    if (listen_fd < 0) {
        return -1;
    }
    return 0;
}
