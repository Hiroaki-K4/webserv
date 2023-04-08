/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 17:19:28 by hkubo             #+#    #+#             */
/*   Updated: 2023/04/08 21:36:31 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"

ConfigParser::ConfigParser() : OUTSIDE("OUTSIDE"), IN_SERVER("IN_SERVER"), IN_LOCATION("IN_LOCATION") {
    set_state(OUTSIDE);
    set_client_max_body_size(-1);
}

ConfigParser::~ConfigParser() {}

void ConfigParser::set_state(state line_state) { this->line_state = line_state; }

ConfigParser::state ConfigParser::get_state() { return this->line_state; }

void ConfigParser::set_client_max_body_size(int client_max_body_size) { this->client_max_body_size = client_max_body_size; }

int ConfigParser::get_client_max_body_size() { return this->client_max_body_size; }

int ConfigParser::check_client_max_body_size(std::string value) {
    std::string key = "client_max_body_size";
    size_t pos = value.find(key);
    std::string trimed = trim_value(value.substr(pos + key.length()));
    if (trimed.length() <= 0) {
        std::cout << "[ERROR] ConfigParser::check_client_max_body_size: client_max_body_size is invalid" << std::endl;
        return FAILURE;
    }
    if (trimed.at(trimed.length() - 1) == ';') {
        std::string client_size = trimed.substr(0, trimed.length() - 1);
        if (!is_number(client_size)) {
            std::cout << "[ERROR] ConfigParser::check_client_max_body_size: client_max_body_size is invalid" << std::endl;
            return FAILURE;
        }
        set_client_max_body_size(string_to_int(client_size));
        return SUCCESS;
    }

    return SUCCESS;
}

int ConfigParser::parse_outside_line(std::string line) {
    std::string value = trim_value(line);
    size_t pos = 0;
    if (value == "server {") {
        set_state(IN_SERVER);
        return SUCCESS;
    } else if ((pos = value.find("client_max_body_size")) != std::string::npos) {
        return check_client_max_body_size(value);
    } else {
        std::cout << "[ERROR] ConfigParser::parse_outside_line: config line is invalid" << std::endl;
        return FAILURE;
    }
    return SUCCESS;
}

int ConfigParser::check_listen(std::string value) {
    (void)value;
    return SUCCESS;
}

int ConfigParser::check_server_name(std::string value) {
    (void)value;
    return SUCCESS;
}

int ConfigParser::check_location(std::string value) {
    (void)value;
    return SUCCESS;
}

int ConfigParser::parse_server_line(std::string line) {
    std::string value = trim_value(line);
    size_t pos = 0;
    if (value == "}") {
        set_state(OUTSIDE);
        return SUCCESS;
    } else if ((pos = value.find("listen")) != std::string::npos) {
        return check_listen(value);
    } else if ((pos = value.find("server_name")) != std::string::npos) {
        return check_server_name(value);
    } else if ((pos = value.find("location")) != std::string::npos) {
        return check_location(value);
    } else {
        std::cout << "[ERROR] ConfigParser::parse_server_line: server block is invalid" << std::endl;
        return FAILURE;
    }

    return SUCCESS;
}

int ConfigParser::parse_location_line(std::string line) {
    (void)line;
    return SUCCESS;
}

int ConfigParser::parse_config(const std::string file_name) {
    std::string config_info = read_file(config_dir, file_name);
    std::istringstream data(config_info);
    std::string line;
    while (1) {
        std::getline(data, line, '\n');
        std::cout << "line: " << line << std::endl;
        if (data.bad()) {
            std::cout << "[ERROR] ConfigParser::parse_config: getline badbit error" << std::endl;
            return FAILURE;
        } else if (data.eof()) {
            std::cout << "[INFO] ConfigParser::parse_config: Reached EOF" << std::endl;
            break;
        }

        if (line == "") {
            continue;
        }

        if (get_state() == OUTSIDE) {
            std::cout << "line status: " << get_state() << std::endl;
            if (parse_outside_line(line) == FAILURE) {
                return FAILURE;
            }
        } else if (get_state() == IN_SERVER) {
            std::cout << "line status: " << get_state() << std::endl;
            if (parse_server_line(line) == FAILURE) {
                return FAILURE;
            }
        } else if (get_state() == IN_LOCATION) {
            std::cout << "line status: " << get_state() << std::endl;
            if (parse_location_line(line) == FAILURE) {
                return FAILURE;
            }
        } else {
            std::cout << "[ERROR] ConfigParser::parse_config: invalid line status" << std::endl;
            return FAILURE;
        }
    }

    return SUCCESS;
}
