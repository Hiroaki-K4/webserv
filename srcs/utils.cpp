/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 20:56:30 by hkubo             #+#    #+#             */
/*   Updated: 2023/04/22 15:40:03 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

int read_file(const std::string &dir, const std::string &file_name, std::string &content) {
    std::string file_path = dir + file_name;
    std::ifstream ifs(file_path.c_str());
    if (!ifs) {
        std::cout << "[ERROR] read_file: config file is invalid" << std::endl;
        return FAILURE;
    }
    std::stringstream ss;
    ss << ifs.rdbuf();
    content = ss.str();

    return SUCCESS;
}

std::string trim_value(std::string line) {
    size_t begin = line.find_first_not_of(" ");
    if (begin == std::string::npos) return "";

    size_t end = line.find_last_not_of(" ");
    size_t range = end - begin + 1;

    return line.substr(begin, range);
}

bool is_number(const std::string &str) {
    for (unsigned int i = 0; i < str.length(); i++) {
        if (!std::isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

int string_to_int(const std::string &str) {
    std::stringstream ss;
    int num;
    ss << str;
    ss >> num;

    return num;
}
