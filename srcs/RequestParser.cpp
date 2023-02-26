/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 14:35:39 by hkubo             #+#    #+#             */
/*   Updated: 2023/02/26 17:51:55 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestParser.hpp"

RequestParser::RequestParser() : REQ_LINE("REQ_LINE"), REQ_HEADER("REQ_HEADER"), REQ_BODY("REQ_BODY"), GET("GET"), POST("POST"), DELETE("DELETE") {
    std::cout << "Hello, RequestParser." << std::endl;
    set_state(REQ_LINE);
    std::cout << "Initial line state: " << get_state() << std::endl;
}

RequestParser::~RequestParser() { std::cout << "Goodbye, RequestParser." << std::endl; }

void RequestParser::set_state(RequestParser::state line_state) { this->line_state = line_state; }

RequestParser::state RequestParser::get_state() { return this->line_state; }

void RequestParser::set_request(const std::string request) { this->request = request; }

std::string RequestParser::get_request() { return this->request; }

int RequestParser::parse_request(const std::string request) {
    set_request(request);

    return 0;
}