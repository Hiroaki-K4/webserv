/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 22:12:06 by hkubo             #+#    #+#             */
/*   Updated: 2023/02/05 23:03:00 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int open_listenfd(char *port)
{
    std::cout << "Port: " << port << std::endl;
    // TODO: Add socket() for making socket.
    return 0;
}

int main(int argc, char *argv[])
{
    int listenfd;

    if (argc != 2) {
        std::cout << "[main][ERROR] Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }
    listenfd = open_listenfd(argv[1]);
    std::cout << "listenfd: " << listenfd << std::endl;
    return 0;
}