/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_request_parser.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 08:18:00 by hkubo             #+#    #+#             */
/*   Updated: 2023/03/05 20:56:48 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>
#include <fstream>
#include "RequestParser.hpp"

std::string read_file(const std::string& file_name) {
    std::string file_path = "test/request_parser/cases/" + file_name;
    std::ifstream ifs(file_path.c_str());
    std::stringstream ss;
    ss << ifs.rdbuf();
    std::string file_content = ss.str();

    return file_content;
}

TEST(RequestParser, get_normal1) {
    std::string content = read_file("get_normal1.txt");
    RequestParser parser;
    parser.parse_request(content);

    EXPECT_EQ(false, parser.get_is_error_request());
    EXPECT_EQ(parser.GET, parser.get_request_method());
    EXPECT_EQ("/", parser.get_target_uri());
    EXPECT_EQ(parser.HTTP_VERSION, parser.get_http_version());
}

TEST(RequestParser, post_normal1) {
    std::string content = read_file("post_normal1.txt");
    RequestParser parser;
    parser.parse_request(content);

    EXPECT_EQ(false, parser.get_is_error_request());
    EXPECT_EQ(parser.POST, parser.get_request_method());
    EXPECT_EQ("/", parser.get_target_uri());
    EXPECT_EQ(parser.HTTP_VERSION, parser.get_http_version());
}

TEST(RequestParser, delete_normal1) {
    std::string content = read_file("delete_normal1.txt");
    RequestParser parser;
    parser.parse_request(content);

    EXPECT_EQ(false, parser.get_is_error_request());
    EXPECT_EQ(parser.DELETE, parser.get_request_method());
    EXPECT_EQ("/index.html", parser.get_target_uri());
    EXPECT_EQ(parser.HTTP_VERSION, parser.get_http_version());
}
