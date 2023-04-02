/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 17:19:52 by hkubo             #+#    #+#             */
/*   Updated: 2023/04/02 21:04:19 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <vector>

#include "ServerConfig.hpp"

#define SUCCESS 0
#define FAILURE -1

class ConfigParser {
   public:
    ConfigParser();
    ~ConfigParser();
    int read_config(char *file_name);

   private:
    std::vector<ServerConfig> servers;
};

#endif
