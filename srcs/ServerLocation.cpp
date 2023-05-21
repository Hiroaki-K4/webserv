/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerLocation.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 16:58:29 by hkubo             #+#    #+#             */
/*   Updated: 2023/05/21 16:20:24 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerLocation.hpp"

ServerLocation::ServerLocation() : is_cgi(false), autoindex(false) {}

ServerLocation::~ServerLocation() {}

void ServerLocation::set_route(const std::string route) { this->route = route; }

std::string ServerLocation::get_route() { return this->route; }

void ServerLocation::set_alias(const std::string alias) { this->alias = alias; }

std::string ServerLocation::get_alias() { return this->alias; }

void ServerLocation::set_root(const std::string root) { this->root = root; }

std::string ServerLocation::get_root() { return this->root; }

void ServerLocation::set_is_cgi(bool is_cgi) { this->is_cgi = is_cgi; }

bool ServerLocation::get_is_cgi() { return this->is_cgi; }

void ServerLocation::set_index(const std::string index) { this->index = index; }

std::string ServerLocation::get_index() { return this->index; }

void ServerLocation::set_error_pages(const std::map<int, std::string> error_pages) { this->error_pages = error_pages; }

std::map<int, std::string> ServerLocation::get_error_pages() { return this->error_pages; }

void ServerLocation::set_allow_method(const std::vector<std::string> allow_method) { this->allow_method = allow_method; }

std::vector<std::string> ServerLocation::get_allow_method() { return this->allow_method; }

void ServerLocation::set_autoindex(bool autoindex) { this->autoindex = autoindex; }

bool ServerLocation::get_autoindex() { return this->autoindex; }
