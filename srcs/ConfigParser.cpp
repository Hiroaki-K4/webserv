/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 17:19:28 by hkubo             #+#    #+#             */
/*   Updated: 2023/04/09 18:06:51 by hkubo            ###   ########.fr       */
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

void ConfigParser::set_servers(std::vector<ServerConfig *> servers) { this->servers = servers; }

std::vector<ServerConfig *> ConfigParser::get_servers() { return this->servers; }

void ConfigParser::set_client_max_body_size(int client_max_body_size) { this->client_max_body_size = client_max_body_size; }

int ConfigParser::get_client_max_body_size() { return this->client_max_body_size; }

void ConfigParser::add_server(ServerConfig *server) {
    std::vector<ServerConfig *> new_servers;
    new_servers.push_back(server);
    set_servers(new_servers);
}

int ConfigParser::extract_config_string(std::string value, std::string key, std::string &result) {
    size_t pos = value.find(key);
    std::string trimed = trim_value(value.substr(pos + key.length()));
    if (trimed.length() <= 0) {
        std::cout << "[ERROR] ConfigParser::extract_config_string: config line is invalid" << std::endl;
        return FAILURE;
    }

    if (trimed.at(trimed.length() - 1) == ';') {
        result = trimed.substr(0, trimed.length() - 1);
    } else {
        result = trimed;
    }

    return SUCCESS;
}

int ConfigParser::extract_config_number(std::string value, std::string key, int &result) {
    size_t pos = value.find(key);
    std::string trimed = trim_value(value.substr(pos + key.length()));
    if (trimed.length() <= 0) {
        std::cout << "[ERROR] ConfigParser::extract_config_number: config line is invalid" << std::endl;
        return FAILURE;
    }

    std::string num;
    if (trimed.at(trimed.length() - 1) == ';') {
        num = trimed.substr(0, trimed.length() - 1);
    } else {
        num = trimed;
    }
    if (!is_number(num)) {
        std::cout << "[ERROR] ConfigParser::extract_config_number: config line is invalid" << std::endl;
        return FAILURE;
    }
    result = string_to_int(num);

    return SUCCESS;
}

int ConfigParser::parse_outside_line(std::string line) {
    std::string value = trim_value(line);
    size_t pos = 0;
    if (value == "server {") {
        set_state(IN_SERVER);
        ServerConfig *server = new ServerConfig();
        add_server(server);
        return SUCCESS;
    } else if ((pos = value.find("client_max_body_size")) != std::string::npos) {
        int result;
        int res = extract_config_number(value, "client_max_body_size", result);
        if (res == SUCCESS) {
            set_client_max_body_size(result);
        }
        return res;
    } else {
        std::cout << "[ERROR] ConfigParser::parse_outside_line: config line is invalid" << std::endl;
        return FAILURE;
    }
    return SUCCESS;
}

int ConfigParser::check_location(std::string value) {
    std::string key = "location";
    size_t pos = value.find(key);
    std::string trimed = trim_value(value.substr(pos + key.length()));
    std::cout << "trimed_location: " << trimed << std::endl;
    if (trimed.length() <= 0) {
        std::cout << "[ERROR] ConfigParser::check_location: location is invalid" << std::endl;
        return FAILURE;
    }

    std::string delimiter = " ";
    pos = 0;
    int token_count = 0;
    std::string token;
    std::string route;
    while ((pos = trimed.find(delimiter)) != std::string::npos) {
        if (token_count > 0) {
            std::cout << "[ERROR] ConfigParser::check_location: location is invalid" << std::endl;
            return FAILURE;
        }
        token = trimed.substr(0, pos);
        route = token;
        trimed.erase(0, pos + delimiter.length());
        if (trimed != "{") {
            std::cout << "[ERROR] ConfigParser::check_location: location is invalid" << std::endl;
            return FAILURE;
        }
        token_count += 1;
    }

    ServerLocation *location = new ServerLocation();
    location->set_route(route);
    get_servers()[get_servers().size() - 1]->add_location(location);
    set_state(IN_LOCATION);

    return SUCCESS;
}

int ConfigParser::parse_server_line(std::string line) {
    std::string value = trim_value(line);
    size_t pos = 0;
    if (value == "}") {
        set_state(OUTSIDE);
        return SUCCESS;
    } else if ((pos = value.find("listen")) != std::string::npos) {
        int result;
        int res = extract_config_number(value, "listen", result);
        if (res == SUCCESS) {
            get_servers()[get_servers().size() - 1]->set_port(result);
        }
        return res;
    } else if ((pos = value.find("server_name")) != std::string::npos) {
        std::string result;
        int res = extract_config_string(value, "server_name", result);
        if (res == SUCCESS) {
            get_servers()[get_servers().size() - 1]->set_host_name(result);
        }
        return res;
    } else if ((pos = value.find("location")) != std::string::npos) {
        return check_location(value);
    } else {
        std::cout << "[ERROR] ConfigParser::parse_server_line: server block is invalid" << std::endl;
        return FAILURE;
    }

    return SUCCESS;
}

int ConfigParser::parse_location_line(std::string line) {
    std::string value = trim_value(line);
    size_t pos = 0;
    if (value == "}") {
        set_state(IN_SERVER);
        return SUCCESS;
    } else if ((pos = value.find("alias")) != std::string::npos) {
        std::string result;
        int res = extract_config_string(value, "alias", result);
        if (res == SUCCESS) {
            unsigned int size = get_servers()[get_servers().size() - 1]->get_locations().size();
            get_servers()[get_servers().size() - 1]->get_locations()[size - 1]->set_alias(result);
        }
        return res;
    } else if ((pos = value.find("root")) != std::string::npos) {
        std::string result;
        int res = extract_config_string(value, "root", result);
        if (res == SUCCESS) {
            unsigned int size = get_servers()[get_servers().size() - 1]->get_locations().size();
            get_servers()[get_servers().size() - 1]->get_locations()[size - 1]->set_root(result);
        }
        return res;
    } else if ((pos = value.find("index")) != std::string::npos) {
        std::string result;
        int res = extract_config_string(value, "index", result);
        if (res == SUCCESS) {
            unsigned int size = get_servers()[get_servers().size() - 1]->get_locations().size();
            get_servers()[get_servers().size() - 1]->get_locations()[size - 1]->set_index(result);
        }
        return res;
    } else if ((pos = value.find("error_page")) != std::string::npos) {
        std::string result;
        int res = extract_config_string(value, "error_page", result);
        if (res == SUCCESS) {
            unsigned int size = get_servers()[get_servers().size() - 1]->get_locations().size();
            get_servers()[get_servers().size() - 1]->get_locations()[size - 1]->set_error_page(result);
        }
        return res;
    } else if ((pos = value.find("allow_method")) != std::string::npos) {
        // TODO: Add the process of allow method
    } else {
        std::cout << "[ERROR] ConfigParser::parse_location_line: location block is invalid" << std::endl;
        return FAILURE;
    }

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

    std::cout << "client size: " << get_client_max_body_size() << std::endl;
    std::cout << "server size: " << get_servers().size() << std::endl;
    std::cout << "server port: " << get_servers()[0]->get_port() << std::endl;
    std::cout << "server host: " << get_servers()[0]->get_host_name() << std::endl;
    std::cout << "server route: " << get_servers()[0]->get_locations()[0]->get_route() << std::endl;
    std::cout << "server alias: " << get_servers()[0]->get_locations()[0]->get_alias() << std::endl;
    std::cout << "server root: " << get_servers()[0]->get_locations()[0]->get_root() << std::endl;
    std::cout << "server index: " << get_servers()[0]->get_locations()[0]->get_index() << std::endl;
    std::cout << "server error_page: " << get_servers()[0]->get_locations()[0]->get_error_page() << std::endl;

    return SUCCESS;
}
