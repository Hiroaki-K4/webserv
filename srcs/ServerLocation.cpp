/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerLocation.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 16:58:29 by hkubo             #+#    #+#             */
/*   Updated: 2023/04/09 18:03:23 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerLocation.hpp"

ServerLocation::ServerLocation() {}

ServerLocation::~ServerLocation() {}

void ServerLocation::set_route(std::string route) { this->route = route; }

std::string ServerLocation::get_route() { return this->route; }

void ServerLocation::set_alias(std::string alias) { this->alias = alias; }

std::string ServerLocation::get_alias() { return this->alias; }

void ServerLocation::set_root(std::string root) { this->root = root; }

std::string ServerLocation::get_root() { return this->root; }

void ServerLocation::set_index(std::string index) { this->index = index; }

std::string ServerLocation::get_index() { return this->index; }

void ServerLocation::set_error_page(std::string error_page) { this->error_page = error_page; }

std::string ServerLocation::get_error_page() { return this->error_page; }
