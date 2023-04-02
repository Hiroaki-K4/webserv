/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 17:19:52 by hkubo             #+#    #+#             */
/*   Updated: 2023/04/02 21:24:13 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <vector>

#include "ServerConfig.hpp"
#include "utils.hpp"

#define SUCCESS 0
#define FAILURE -1

const std::string config_dir = "configs/";

class ConfigParser {
   public:
    ConfigParser();
    ~ConfigParser();
    int read_config(const std::string file_name);

   private:
    std::vector<ServerConfig> servers;
};

#endif
