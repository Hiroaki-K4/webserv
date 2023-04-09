/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 17:02:02 by hkubo             #+#    #+#             */
/*   Updated: 2023/04/09 17:18:21 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include <iostream>
#include <vector>

#include "ServerLocation.hpp"

class ServerConfig {
   public:
    ServerConfig();
    ~ServerConfig();
    void set_host_name(const std::string host_name);
    std::string get_host_name();
    void set_port(int port);
    int get_port();
    void set_locations(std::vector<ServerLocation *> locations);
    std::vector<ServerLocation *> get_locations();
    void add_location(ServerLocation *location);

   private:
    std::string host_name;
    int port;
    std::string default_error_page;
    bool is_default_server;
    std::vector<ServerLocation *> locations;
};

#endif
