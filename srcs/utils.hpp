/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 20:56:24 by hkubo             #+#    #+#             */
/*   Updated: 2023/04/08 17:44:28 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <fstream>
#include <sstream>
#include <string>

std::string read_file(const std::string &dir, const std::string &file_name);
std::string trim_value(std::string line);
bool is_number(const std::string &str);
int string_to_int(const std::string &str);

#endif
