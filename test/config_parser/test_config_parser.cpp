/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_config_parser.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 14:36:33 by hkubo             #+#    #+#             */
/*   Updated: 2023/04/16 16:38:59 by hkubo            ###   ########.fr       */
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

TEST(ConfigParser, ok_error_page) {
    ConfigParser config;
    config.parse_config("error_page.conf");
    std::vector<ServerConfig *> servers = config.get_servers();
    std::vector<ServerLocation *> locations = servers[0]->get_locations();

    EXPECT_EQ(1, servers.size());
    EXPECT_EQ(4242, servers[0]->get_port());
    EXPECT_EQ(1, locations.size());
    EXPECT_EQ("/", locations[0]->get_route());
    EXPECT_EQ("./docs/", locations[0]->get_alias());
    EXPECT_EQ(2, locations[0]->get_error_pages().size());
    EXPECT_EQ("/error_page/403.html", locations[0]->get_error_pages()[403]);
    EXPECT_EQ("/error_page/404.html", locations[0]->get_error_pages()[404]);
}

TEST(ConfigParser, ok_multiple_host) {
    ConfigParser config;
    config.parse_config("multiple_host.conf");
    std::vector<ServerConfig *> servers = config.get_servers();
    std::vector<ServerLocation *> locations1 = servers[0]->get_locations();
    std::vector<ServerLocation *> locations2 = servers[1]->get_locations();

    EXPECT_EQ(2, servers.size());

    EXPECT_EQ(80, servers[0]->get_port());
    EXPECT_EQ("example.com", servers[0]->get_host_name());
    EXPECT_EQ(1, locations1.size());
    EXPECT_EQ("/", locations1[0]->get_route());
    EXPECT_EQ("./docs/", locations1[0]->get_alias());

    EXPECT_EQ(80, servers[1]->get_port());
    EXPECT_EQ("webserv.com", servers[1]->get_host_name());
    EXPECT_EQ(1, locations2.size());
    EXPECT_EQ("/", locations2[0]->get_route());
    EXPECT_EQ("./docs/dir/", locations2[0]->get_alias());
}

TEST(ConfigParser, ok_multiple_port) {
    ConfigParser config;
    config.parse_config("multiple_port.conf");
    std::vector<ServerConfig *> servers = config.get_servers();
    std::vector<ServerLocation *> locations1 = servers[0]->get_locations();
    std::vector<ServerLocation *> locations2 = servers[1]->get_locations();

    EXPECT_EQ(2, servers.size());

    EXPECT_EQ(4242, servers[0]->get_port());
    EXPECT_EQ(1, locations1.size());
    EXPECT_EQ("/", locations1[0]->get_route());
    EXPECT_EQ("./docs/", locations1[0]->get_alias());

    EXPECT_EQ(2121, servers[1]->get_port());
    EXPECT_EQ(1, locations2.size());
    EXPECT_EQ("/", locations2[0]->get_route());
    EXPECT_EQ("./docs/dir/", locations2[0]->get_alias());
}

TEST(ConfigParser, ok_multiple_route) {
    ConfigParser config;
    config.parse_config("multiple_route.conf");
    std::vector<ServerConfig *> servers = config.get_servers();
    std::vector<ServerLocation *> locations = servers[0]->get_locations();

    EXPECT_EQ(1, servers.size());

    EXPECT_EQ(4242, servers[0]->get_port());
    EXPECT_EQ(2, locations.size());
    EXPECT_EQ("/a/", locations[0]->get_route());
    EXPECT_EQ("./docs/", locations[0]->get_alias());
    EXPECT_EQ("/b/", locations[1]->get_route());
    EXPECT_EQ("./docs/dir/", locations[1]->get_alias());
}
