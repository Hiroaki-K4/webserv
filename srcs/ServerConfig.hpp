/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 17:02:02 by hkubo             #+#    #+#             */
/*   Updated: 2023/05/06 16:50:14 by hkubo            ###   ########.fr       */
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
    void set_port(const int port);
    int get_port();
    void set_client_max_body_size(int client_max_body_size);
    int get_client_max_body_size();
    void set_locations(const std::vector<ServerLocation *> locations);
    std::vector<ServerLocation *> get_locations();
    void add_location(ServerLocation *location);

   private:
    std::string host_name;
    int port;
    std::string default_error_page;
    bool is_default_server;
    int client_max_body_size;
    std::vector<ServerLocation *> locations;
};

#endif
