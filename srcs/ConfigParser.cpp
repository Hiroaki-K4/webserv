/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 17:19:28 by hkubo             #+#    #+#             */
/*   Updated: 2023/04/08 16:33:42 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"

ConfigParser::ConfigParser() : OUTSIDE("OUTSIDE"), IN_SERVER("IN_SERVER"), IN_LOCATION("IN_LOCATION") { set_state(OUTSIDE); }

ConfigParser::~ConfigParser() {}

void ConfigParser::set_state(state line_state) { this->line_state = line_state; }

ConfigParser::state ConfigParser::get_state() { return this->line_state; }

int ConfigParser::parse_config(const std::string file_name) {
    std::string config_info = read_file(config_dir, file_name);
    // std::cout << config_info << std::endl;
    std::istringstream data(config_info);
    std::string line;
    while (1) {
        std::getline(data, line, '\n');
        std::cout << "line: " << line << std::endl;
        if (data.bad()) {
            std::cout << "[ERROR] ConfigParser::parse_config: getline badbit error" << std::endl;
            return FAILURE;
        }

        if (get_state() == OUTSIDE) {
            std::cout << "line status: " << get_state() << std::endl;
        } else if (get_state() == IN_SERVER) {
            std::cout << "line status: " << get_state() << std::endl;
        } else if (get_state() == IN_SERVER) {
            std::cout << "line status: " << get_state() << std::endl;
        } else {
            std::cout << "[ERROR] ConfigParser::parse_config: invalid line status" << std::endl;
            return FAILURE;
        }

        if (data.eof()) {
            std::cout << "[INFO] ConfigParser::parse_config: Reached EOF" << std::endl;
            break;
        }
    }

    return SUCCESS;
}
