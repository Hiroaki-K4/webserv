/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerLocation.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 16:57:56 by hkubo             #+#    #+#             */
/*   Updated: 2023/04/09 21:23:43 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERLOCATION_HPP
#define SERVERLOCATION_HPP

#include <iostream>
#include <vector>

class ServerLocation {
   public:
    ServerLocation();
    ~ServerLocation();
    void set_route(const std::string route);
    std::string get_route();
    void set_alias(const std::string alias);
    std::string get_alias();
    void set_root(const std::string root);
    std::string get_root();
    void set_index(const std::string index);
    std::string get_index();
    void set_error_page(const std::string error_page);
    std::string get_error_page();
    void set_allow_method(const std::vector<std::string> allow_method);
    std::vector<std::string> get_allow_method();

   private:
    std::string route;
    std::string alias;
    std::string root;
    std::string index;
    std::string error_page;
    std::vector<std::string> allow_method;
};

#endif
