/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 14:35:39 by hkubo             #+#    #+#             */
/*   Updated: 2023/02/28 09:36:32 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

#include <iostream>
#include <map>

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

    RequestParser();
    ~RequestParser();
    void set_state(state line_state);
    state get_state();
    void set_request(const std::string request);
    std::string get_request();
    void set_is_error_request(bool is_error);
    bool get_is_error_request();
    int parse_request(const std::string request);


   private:
    std::string request;
    state line_state;
    method request_method;
    std::string target_path;
    std::string host;
    std::map<std::string, std::string> header;

    bool is_error_request;
};

#endif
