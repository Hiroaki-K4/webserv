/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_config_parser.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 14:36:33 by hkubo             #+#    #+#             */
/*   Updated: 2023/04/15 17:20:14 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>
#include <fstream>
#include "ConfigParser.hpp"
#include "utils.hpp"

TEST(ConfigParser, ok_default) {
    ConfigParser config;
    config.parse_config("default.conf");
    std::vector<ServerConfig *> servers = config.get_servers();
    std::vector<ServerLocation *> locations = servers[0]->get_locations();

    EXPECT_EQ(59, config.get_client_max_body_size());
    EXPECT_EQ(1, servers.size());
    EXPECT_EQ(8080, servers[0]->get_port());
    EXPECT_EQ("example.com", servers[0]->get_host_name());
    EXPECT_EQ(1, locations.size());
    EXPECT_EQ("/", locations[0]->get_route());
    EXPECT_EQ("./docs/", locations[0]->get_alias());
    EXPECT_EQ("/var/www/html", locations[0]->get_root());
    EXPECT_EQ("index.html", locations[0]->get_index());
    EXPECT_EQ(1, locations[0]->get_error_pages().size());
    EXPECT_EQ("/error_page/404.html", locations[0]->get_error_pages()[404]);
    EXPECT_EQ(2, locations[0]->get_allow_method().size());
    EXPECT_EQ("GET", locations[0]->get_allow_method()[0]);
    EXPECT_EQ("POST", locations[0]->get_allow_method()[1]);
}
