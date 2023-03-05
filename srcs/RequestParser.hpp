/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 14:35:39 by hkubo             #+#    #+#             */
/*   Updated: 2023/03/05 20:47:28 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

#include <cstdlib>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

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

    RequestParser();
    ~RequestParser();
    void set_state(state line_state);
    state get_state();
    void set_request_method(method method);
    method get_request_method();
    void set_target_uri(const std::string token);
    const std::string get_target_uri();
    void set_http_version(const std::string request);
    const std::string get_http_version();
    void set_request(const std::string request);
    std::string get_request();
    void set_is_error_request(bool is_error);
    bool get_is_error_request();
    void set_header(const std::map<std::string, std::string> header);
    const std::map<std::string, std::string> get_header();

    int handle_request_method(const std::string token);
    int handle_target_uri(const std::string token);
    int handle_http_version(const std::string token);

    int parse_request(const std::string request);
    int parse_request_line(std::string line);
    int parse_request_header(std::string line);
    int parse_request_body(std::string line);

   private:
    std::string request;
    state line_state;
    method request_method;
    std::string target_uri;
    std::string http_version;
    std::string host;
    std::map<std::string, std::string> header;

    bool is_error_request;
};

#endif
