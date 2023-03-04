/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 14:35:39 by hkubo             #+#    #+#             */
/*   Updated: 2023/03/04 16:32:26 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestParser.hpp"

RequestParser::RequestParser() : REQ_LINE("REQ_LINE"), REQ_HEADER("REQ_HEADER"), REQ_BODY("REQ_BODY"), GET("GET"), POST("POST"), DELETE("DELETE") {
    set_state(REQ_LINE);
    set_is_error_request(false);
    std::cout << "Initial value of RequestParser class" << std::endl;
    std::cout << "state: " << get_state() << std::endl;
    std::cout << "is_error_request: " << get_is_error_request() << std::endl;
}

RequestParser::~RequestParser() { std::cout << "Goodbye, RequestParser." << std::endl; }

void RequestParser::set_state(RequestParser::state line_state) { this->line_state = line_state; }

RequestParser::state RequestParser::get_state() { return this->line_state; }

void RequestParser::set_request(const std::string request) { this->request = request; }

std::string RequestParser::get_request() { return this->request; }

void RequestParser::set_is_error_request(bool is_error) { this->is_error_request = is_error; }

bool RequestParser::get_is_error_request() { return this->is_error_request; }

int RequestParser::parse_request(const std::string request) {
    set_request(request);

    // Readline until EOF
    std::istringstream data(request);
    std::string name;
    while (1) {
        std::getline(data, name, '\n');
        std::cout << "name: " << name << std::endl;
        if (data.eof()) {
            std::cout << "[INFO] RequestParser::parse_request: Reached EOF" << std::endl;
            break;
        }
        if (data.fail()) {
            std::cout << "[ERROR] RequestParser::parse_request: getline error" << std::endl;
            break;
        }
    }

    // Check line_state and decide parse method
    // parse_request_line, parse_request_header, parse_request_body

    return 0;
}