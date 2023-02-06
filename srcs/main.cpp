/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 22:12:06 by hkubo             #+#    #+#             */
/*   Updated: 2023/02/06 23:05:18 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int open_listen_fd(char *port)
{
    std::cout << "Port: " << port << std::endl;
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        std::cout << "[main.cpp open_listen_fd][ERROR] socket function failed." << std::endl;
        return -1;
    }

    // The option is set so that connection requests can be accepted immediately after rebooting.
    int opt_val = 1;
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt_val, sizeof(int)) == -1) {
        return -1;
        std::cout << "[main.cpp open_listen_fd][ERROR] setsockopt function failed." << std::endl;
    }

    struct sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    *addr.sin_family = AF_INET;
    // myname.sin_addr.s_addr = inet_addr("129.5.24.1");
    // /* specific interface */
    // myname.sin_port = htons(1024);
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
    return 0;
}
