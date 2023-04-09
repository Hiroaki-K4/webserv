/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerLocation.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 16:58:29 by hkubo             #+#    #+#             */
/*   Updated: 2023/04/09 17:02:42 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerLocation.hpp"

ServerLocation::ServerLocation() {}

ServerLocation::~ServerLocation() {}

void ServerLocation::set_route(std::string route) { this->route = route; }

std::string ServerLocation::get_route() { return this->route; }
