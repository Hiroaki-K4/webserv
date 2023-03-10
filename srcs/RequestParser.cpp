/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 14:35:39 by hkubo             #+#    #+#             */
/*   Updated: 2023/03/12 17:26:49 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestParser.hpp"

RequestParser::RequestParser()
    : REQ_LINE("REQ_LINE"), REQ_HEADER("REQ_HEADER"), REQ_BODY("REQ_BODY"), GET("GET"), POST("POST"), DELETE("DELETE"), HTTP_VERSION("HTTP/1.1"), ENCODING("ENCODING"), RAW("RAW") {
    set_state(REQ_LINE);
    set_is_error_request(false);
}

RequestParser::~RequestParser() { std::cout << "Goodbye, RequestParser." << std::endl; }

void RequestParser::set_state(RequestParser::state line_state) { this->line_state = line_state; }

RequestParser::state RequestParser::get_state() { return this->line_state; }

void RequestParser::set_request_method(method method) { this->request_method = method; }

RequestParser::method RequestParser::get_request_method() { return this->request_method; }

void RequestParser::set_body_type(body_type request_body_type) { this->request_body_type = request_body_type; }

RequestParser::body_type RequestParser::get_body_type() { return this->request_body_type; }

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

void RequestParser::set_body(const std::string body) { this->body = body; }

std::string RequestParser::get_body() { return this->body; }

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
        return EXIT_SUCCESS;
    } else {
        std::cout << "[ERROR] RequestParser::parse_request_header: The request header is invalid" << std::endl;
        set_is_error_request(true);
        return EXIT_FAILURE;
    }
}

int RequestParser::parse_request_body(const std::string request, unsigned int line_count) {
    std::istringstream data(request);
    std::string line;
    unsigned int curr_line = 0;
    while (1) {
        std::getline(data, line, '\n');
        curr_line += 1;
        if (curr_line == line_count) {
            break;
        }
    }
    if (get_body_type() == RAW) {
        int content_len;
        std::istringstream(get_header().at("Content-Length")) >> content_len;
        char buf[content_len + 1];

        data.read(buf, content_len);
        size_t readed = data.gcount();
        buf[readed] = '\0';
        set_body(buf);
        return EXIT_SUCCESS;
    } else if (get_body_type() == ENCODING) {
        unsigned int chunk_size;
        unsigned int length = 0;
        std::string body;
        while (1) {
            std::getline(data, line, '\n');
            if (line == "0") {
                std::cout << "[INFO] RequestParser::parse_request_body: Reached last chunk" << std::endl;
                break;
            }

            std::stringstream ss;
            ss << std::hex << line;
            ss >> chunk_size;
            length += chunk_size;
            std::getline(data, line, '\n');
            body = body + line.substr(0, chunk_size);

            if (data.eof()) {
                std::cout << "[INFO] RequestParser::parse_request_body: Reached EOF" << std::endl;
                break;
            }
        }
        set_body(body);
        std::stringstream ss;
        ss << length;
        std::string content_length = ss.str();
        set_header("Content-Length", content_length);
        return EXIT_SUCCESS;
    } else {
        std::cout << "[ERROR] RequestParser::parse_request_body: the type of request body is invalid" << std::endl;
        set_is_error_request(true);
        return EXIT_FAILURE;
    }
}

bool RequestParser::is_valid_header() {
    const std::map<std::string,std::string> m = get_header();
    if (m.find("Transfer-Encoding") != m.end() && m.find("Content-Length") != m.end()) {
        set_is_error_request(true);
        std::cout << "[ERROR] RequestParser::is_valid_header: the value of header is invalid" << std::endl;
        return false;
    }

    return true;
}

bool RequestParser::is_include_request_body() {
    const std::map<std::string,std::string> m = get_header();
    if (m.find("Transfer-Encoding") != m.end()) {
        set_body_type(ENCODING);
        return true;
    }
    if (m.find("Content-Length") != m.end()) {
        set_body_type(RAW);
        return true;
    }

    return false;
}

int RequestParser::parse_request(const std::string request) {
    set_request(request);

    std::istringstream data(request);
    std::string line;
    unsigned int line_count = 0;
    // Read requst line and header
    while (1) {
        line_count += 1;
        std::getline(data, line, '\n');
        if (data.bad()) {
            std::cout << "[ERROR] RequestParser::parse_request: getline badbit error" << std::endl;
            set_is_error_request(true);
            return EXIT_FAILURE;
        } else if (data.fail()) {
            return EXIT_SUCCESS;
        }

        if (line == "") {
            break;
        }

        // Check line_state and decide parse method
        if (get_state() == REQ_LINE) {
            if (parse_request_line(line) == EXIT_FAILURE) {
                return EXIT_FAILURE;
            }
            set_state(REQ_HEADER);
        } else if (get_state() == REQ_HEADER) {
            if (parse_request_header(line) == EXIT_FAILURE) {
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

    // Decide whether to parse request body
    if (!data.eof() && get_state() == REQ_HEADER && line == "") {
        if (is_valid_header()) {
            if (is_include_request_body()) {
                set_state(REQ_BODY);
                parse_request_body(request, line_count);
                return EXIT_SUCCESS;
            } else {
                return EXIT_SUCCESS;
            }
        } else {
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
