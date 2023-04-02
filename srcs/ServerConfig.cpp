/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 17:01:39 by hkubo             #+#    #+#             */
/*   Updated: 2023/04/02 17:18:05 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

ServerConfig::ServerConfig() {}

ServerConfig::~ServerConfig() {}

void ServerConfig::set_host_name(const std::string host_name) { this->host_name = host_name; }

std::string ServerConfig::get_host_name() { return this->host_name; }

void ServerConfig::set_port(const std::string port) { this->port = port; }

std::string ServerConfig::get_port() { return this->port; }
