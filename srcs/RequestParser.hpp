/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 14:35:39 by hkubo             #+#    #+#             */
/*   Updated: 2023/03/19 14:21:27 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

#include <cstdlib>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <cmath>

class RequestParser {
   public:
    typedef std::string state;
    const state REQ_LINE;
    const state REQ_HEADER;
    const state REQ_BODY;

    typedef std::string method;
    const method GET;
    const method POST;
    const method DELETE;

    const std::string HTTP_VERSION;

    typedef std::string body_type;
    const body_type ENCODING;
    const body_type RAW;

    RequestParser();
    ~RequestParser();
    void set_state(state line_state);
    state get_state();
    void set_request_method(method method);
    method get_request_method();
    void set_body_type(body_type request_body_type);
    body_type get_body_type();
    void set_target_uri(const std::string token);
    const std::string get_target_uri();
    void set_http_version(const std::string request);
    const std::string get_http_version();
    void set_request(const std::string request);
    std::string get_request();
    void set_is_error_request(bool is_error);
    bool get_is_error_request();
    void set_header(const std::string name, const std::string value);
    const std::map<std::string, std::string> get_header();
    void set_body(const std::string body);
    std::string get_body();

    int handle_request_method(const std::string token);
    int handle_target_uri(const std::string token);
    int handle_http_version(const std::string token);
    std::string trim_value(std::string line);
    bool is_valid_header();
    bool is_include_request_body();

    int parse_request(const std::string request);
    int parse_request_line(std::string line);
    int parse_request_header(std::string line);
    int parse_request_body(const std::string request, unsigned int line_count);


   private:
    std::string request;
    state line_state;
    method request_method;
    body_type request_body_type;
    std::string target_uri;
    std::string http_version;
    std::string host;
    std::map<std::string, std::string> header;
    std::string body;

    bool is_error_request;
};

#endif
