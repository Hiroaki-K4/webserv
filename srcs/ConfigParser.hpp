/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 17:19:52 by hkubo             #+#    #+#             */
/*   Updated: 2023/04/09 14:52:14 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <iostream>
#include <string>
#include <vector>

#include "ServerConfig.hpp"
#include "utils.hpp"

#define SUCCESS 0
#define FAILURE -1

const std::string config_dir = "configs/";

class ConfigParser {
   public:
    typedef std::string state;
    const state OUTSIDE;
    const state IN_SERVER;
    const state IN_LOCATION;

    ConfigParser();
    ~ConfigParser();
    int parse_config(const std::string file_name);
    void set_state(state line_state);
    state get_state();
    void set_servers(std::vector<ServerConfig *> servers);
    std::vector<ServerConfig *> get_servers();
    void set_client_max_body_size(int client_max_body_size);
    void add_server(ServerConfig *server);
    int get_client_max_body_size();
    int check_client_max_body_size(std::string value);
    int check_listen(std::string value);
    int check_server_name(std::string value);
    int check_location(std::string value);
    int parse_outside_line(std::string line);
    int parse_server_line(std::string line);
    int parse_location_line(std::string line);

   private:
    std::vector<ServerConfig *> servers;
    state line_state;
    int client_max_body_size;
};

#endif
