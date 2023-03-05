/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_request_parser.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 08:18:00 by hkubo             #+#    #+#             */
/*   Updated: 2023/03/05 22:08:18 by hkubo            ###   ########.fr       */
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

TEST(RequestParser, ok_normal_get_01) {
    std::string content = read_file("ok_normal_get_01.txt");
    RequestParser parser;
    parser.parse_request(content);

    // Chect request line
    EXPECT_EQ(false, parser.get_is_error_request());
    EXPECT_EQ(parser.GET, parser.get_request_method());
    EXPECT_EQ("/", parser.get_target_uri());
    EXPECT_EQ(parser.HTTP_VERSION, parser.get_http_version());

    // Check request header
    EXPECT_EQ("localhost", parser.get_header().at("Host"));
}

TEST(RequestParser, ok_normal_post_01) {
    std::string content = read_file("ok_normal_post_01.txt");
    RequestParser parser;
    parser.parse_request(content);

    // Chect request line
    EXPECT_EQ(false, parser.get_is_error_request());
    EXPECT_EQ(parser.POST, parser.get_request_method());
    EXPECT_EQ("/", parser.get_target_uri());
    EXPECT_EQ(parser.HTTP_VERSION, parser.get_http_version());

    // Check request header
    EXPECT_EQ("curl/7.16.3 libcurl/7.16.3 OpenSSL/0.9.7l zlib/1.2.3", parser.get_header().at("User-Agent"));
    EXPECT_EQ("www.example.com", parser.get_header().at("Host"));
    EXPECT_EQ("en, mi", parser.get_header().at("Accept-Language"));
    EXPECT_EQ("Mon, 27 Jul 2009 12:28:53 GMT", parser.get_header().at("Date"));
    EXPECT_EQ("51", parser.get_header().at("Content-Length"));
    EXPECT_EQ("text/plain", parser.get_header().at("Content-Type"));
}

TEST(RequestParser, ok_normal_delete_01) {
    std::string content = read_file("ok_normal_delete_01.txt");
    RequestParser parser;
    parser.parse_request(content);

    // Chect request line
    EXPECT_EQ(false, parser.get_is_error_request());
    EXPECT_EQ(parser.DELETE, parser.get_request_method());
    EXPECT_EQ("/index.html", parser.get_target_uri());
    EXPECT_EQ(parser.HTTP_VERSION, parser.get_http_version());

    // Check request header
    EXPECT_EQ("localhost", parser.get_header().at("Host"));
}
