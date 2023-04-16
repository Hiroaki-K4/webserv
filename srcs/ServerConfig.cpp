/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 17:01:39 by hkubo             #+#    #+#             */
/*   Updated: 2023/04/16 16:44:50 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

ServerConfig::ServerConfig() {
    set_host_name("localhost");
    set_port(8080);
}

ServerConfig::~ServerConfig() {}

void ServerConfig::set_host_name(const std::string host_name) { this->host_name = host_name; }

std::string ServerConfig::get_host_name() { return this->host_name; }

void ServerConfig::set_port(const int port) { this->port = port; }

int ServerConfig::get_port() { return this->port; }

void ServerConfig::set_locations(const std::vector<ServerLocation *> locations) { this->locations = locations; }

std::vector<ServerLocation *> ServerConfig::get_locations() { return this->locations; }

void ServerConfig::add_location(ServerLocation *location) {
    std::vector<ServerLocation *> new_locations;
    std::vector<ServerLocation *> exist_locations = get_locations();
    for (std::vector<ServerLocation *>::iterator it = exist_locations.begin(); it != exist_locations.end(); ++it) {
        new_locations.push_back(*it);
    }
    new_locations.push_back(location);
    set_locations(new_locations);
}
