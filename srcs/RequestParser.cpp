/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 14:35:39 by hkubo             #+#    #+#             */
/*   Updated: 2023/03/05 22:05:19 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestParser.hpp"

RequestParser::RequestParser()
    : REQ_LINE("REQ_LINE"), REQ_HEADER("REQ_HEADER"), REQ_BODY("REQ_BODY"), GET("GET"), POST("POST"), DELETE("DELETE"), HTTP_VERSION("HTTP/1.1") {
    set_state(REQ_LINE);
    set_is_error_request(false);
    std::cout << "Initial value of RequestParser class" << std::endl;
    std::cout << "state: " << get_state() << std::endl;
    std::cout << "is_error_request: " << get_is_error_request() << std::endl;
}

RequestParser::~RequestParser() { std::cout << "Goodbye, RequestParser." << std::endl; }

void RequestParser::set_state(RequestParser::state line_state) { this->line_state = line_state; }

RequestParser::state RequestParser::get_state() { return this->line_state; }

void RequestParser::set_request_method(method method) { this->request_method = method; }

RequestParser::method RequestParser::get_request_method() { return this->request_method; }

void RequestParser::set_target_uri(const std::string token) { this->target_uri = token; }

const std::string RequestParser::get_target_uri() { return this->target_uri; }

void RequestParser::set_http_version(const std::string token) { this->http_version = token; }

const std::string RequestParser::get_http_version() { return this->http_version; }

void RequestParser::set_request(const std::string request) { this->request = request; }

std::string RequestParser::get_request() { return this->request; }

void RequestParser::set_is_error_request(bool is_error) { this->is_error_request = is_error; }

bool RequestParser::get_is_error_request() { return this->is_error_request; }

void RequestParser::set_header(const std::string name, const std::string value) { this->header[name] = value; }

const std::map<std::string, std::string> RequestParser::get_header() { return this->header; }

int RequestParser::handle_request_method(const std::string token) {
    if (token == GET || token == POST || token == DELETE) {
        set_request_method(token);
    } else {
        std::cout << "[ERROR] RequestParser::check_request_method: There is no request method" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int RequestParser::handle_target_uri(const std::string token) {
    set_target_uri(token);
    return EXIT_SUCCESS;
}

int RequestParser::handle_http_version(const std::string token) {
    if (token == HTTP_VERSION) {
        set_http_version(token);
    } else {
        std::cout << "[ERROR] RequestParser::handle_http_version: Http version is invalid" << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int RequestParser::parse_request_line(std::string line) {
    std::string delimiter = " ";
    size_t pos = 0;
    int token_count = 0;
    std::string token;
    while ((pos = line.find(delimiter)) != std::string::npos) {
        token = line.substr(0, pos);
        std::cout << "token " << token << " token_count " << token_count << std::endl;
        if (token_count == 0) {
            if (handle_request_method(token) == EXIT_FAILURE) {
                set_is_error_request(true);
                break;
            }
        } else if (token_count == 1) {
            if (handle_target_uri(token) == EXIT_FAILURE) {
                set_is_error_request(true);
                break;
            }
        } else {
            std::cout << "[ERROR] RequestParser::parse_request_line: The request line is too long" << std::endl;
            set_is_error_request(true);
        }
        line.erase(0, pos + delimiter.length());
        token_count += 1;
    }
    if (get_is_error_request()) {
        return EXIT_FAILURE;
    }
    if (handle_http_version(line) == EXIT_FAILURE) {
        set_is_error_request(true);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

std::string RequestParser::trim_value(std::string line) {
    size_t begin = line.find_first_not_of(" ");
    if (begin == std::string::npos)
        return "";

    size_t end = line.find_last_not_of(" ");
    size_t range = end - begin + 1;

    return line.substr(begin, range);
}

int RequestParser::parse_request_header(std::string line) {
    std::string delimiter = ":";
    size_t pos = line.find(delimiter);
    if (pos != std::string::npos && pos > 0 && line[pos - 1] != ' ') {
        std::string name = line.substr(0, pos);
        line.erase(0, pos + delimiter.length());
        std::string value = trim_value(line);
        set_header(name, value);
        std::cout << "name: " << name << " value: " << value << std::endl;
        return EXIT_SUCCESS;
    } else {
        std::cout << "[ERROR] RequestParser::parse_request_header: The request header is invalid" << std::endl;
        set_is_error_request(true);
        return EXIT_FAILURE;
    }
}

int RequestParser::parse_request_body(std::string line) {
    (void)line;
    return EXIT_SUCCESS;
}

int RequestParser::parse_request(const std::string request) {
    set_request(request);

    // Readline until EOF
    std::istringstream data(request);
    std::string line;
    while (1) {
        std::getline(data, line, '\n');
        if (data.fail()) {
            std::cout << "[ERROR] RequestParser::parse_request: getline error" << std::endl;
            return EXIT_FAILURE;
        }

        // Check line_state and decide parse method
        if (get_state() == REQ_LINE) {
            if (parse_request_line(line) == EXIT_FAILURE) {
                return EXIT_FAILURE;
            }
            set_state(REQ_HEADER);
        } else if (get_state() == REQ_HEADER) {
            if (line == "") {
                set_state(REQ_BODY);
                continue;
            }
            if (parse_request_header(line) == EXIT_FAILURE) {
                return EXIT_FAILURE;
            }
        } else if (get_state() == REQ_BODY) {
            if (parse_request_body(line) == EXIT_FAILURE) {
                return EXIT_FAILURE;
            }
        } else {
            std::cout << "[ERROR] RequestParser::parse_request: line state is invaliad" << std::endl;
        }

        if (data.eof()) {
            std::cout << "[INFO] RequestParser::parse_request: Reached EOF" << std::endl;
            break;
        }
    }

    return EXIT_SUCCESS;
}
