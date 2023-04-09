/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerLocation.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 16:57:56 by hkubo             #+#    #+#             */
/*   Updated: 2023/04/09 17:05:32 by hkubo            ###   ########.fr       */
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
    void set_route(std::string route);
    std::string get_route();

   private:
    std::string route;
    std::string alias;
    std::string root;
    std::string index;
    std::string error_page;
    std::vector<std::string> allow_method;
};

#endif
