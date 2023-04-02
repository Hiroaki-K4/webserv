/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 17:02:02 by hkubo             #+#    #+#             */
/*   Updated: 2023/04/02 17:14:42 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include <iostream>

class ServerConfig {
   public:
    ServerConfig();
    ~ServerConfig();
    void set_host_name(const std::string host_name);
    std::string get_host_name();
    void set_port(const std::string port);
    std::string get_port();


   private:
    std::string host_name;
    std::string port;
    std::string default_error_page;
    unsigned int client_body_size_limit;
};

#endif
