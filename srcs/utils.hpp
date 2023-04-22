/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 20:56:24 by hkubo             #+#    #+#             */
/*   Updated: 2023/04/22 15:40:16 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

#define SUCCESS 0
#define FAILURE -1

int read_file(const std::string &dir, const std::string &file_name, std::string &content);
std::string trim_value(std::string line);
bool is_number(const std::string &str);
int string_to_int(const std::string &str);

#endif
