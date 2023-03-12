/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_request_parser.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 08:18:00 by hkubo             #+#    #+#             */
/*   Updated: 2023/03/12 17:25:09 by hkubo            ###   ########.fr       */
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
    int res = parser.parse_request(content);
    EXPECT_EQ(EXIT_SUCCESS, res);

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
    int res = parser.parse_request(content);
    EXPECT_EQ(EXIT_SUCCESS, res);

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
    EXPECT_EQ("68", parser.get_header().at("Content-Length"));
    EXPECT_EQ("text/plain", parser.get_header().at("Content-Type"));
    EXPECT_EQ(parser.RAW, parser.get_body_type());

    // Check request body;
    std::string body = "<html>\n    <body>\n        <h1>Hello, World!</h1>\n    </body>\n</html>";
    EXPECT_EQ(body, parser.get_body());
}

TEST(RequestParser, ok_normal_delete_01) {
    std::string content = read_file("ok_normal_delete_01.txt");
    RequestParser parser;
    int res = parser.parse_request(content);
    EXPECT_EQ(EXIT_SUCCESS, res);

    // Chect request line
    EXPECT_EQ(false, parser.get_is_error_request());
    EXPECT_EQ(parser.DELETE, parser.get_request_method());
    EXPECT_EQ("/index.html", parser.get_target_uri());
    EXPECT_EQ(parser.HTTP_VERSION, parser.get_http_version());

    // Check request header
    EXPECT_EQ("localhost", parser.get_header().at("Host"));
}

TEST(RequestParser, ok_post_with_chunk_data) {
    std::string content = read_file("ok_post_with_chunk_data.txt");
    RequestParser parser;
    int res = parser.parse_request(content);
    EXPECT_EQ(EXIT_SUCCESS, res);

    // Chect request line
    EXPECT_EQ(false, parser.get_is_error_request());
    EXPECT_EQ(parser.POST, parser.get_request_method());
    EXPECT_EQ("/index.html", parser.get_target_uri());
    EXPECT_EQ(parser.HTTP_VERSION, parser.get_http_version());

    // Check request header
    EXPECT_EQ("localhost:8080", parser.get_header().at("Host"));
    EXPECT_EQ(parser.ENCODING, parser.get_body_type());
    EXPECT_EQ("225", parser.get_header().at("Content-Length"));

    // Checkt request body
    EXPECT_EQ("123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901123456789012123456789012312345678901234123456789012345",
        parser.get_body());
}

TEST(RequestParser, ng_request_header_01) {
    std::string content = read_file("ng_request_header_01.txt");
    RequestParser parser;
    int res = parser.parse_request(content);
    EXPECT_EQ(EXIT_FAILURE, res);

    EXPECT_EQ(true, parser.get_is_error_request());
}

TEST(RequestParser, ng_request_header_02) {
    std::string content = read_file("ng_request_header_02.txt");
    RequestParser parser;
    int res = parser.parse_request(content);
    EXPECT_EQ(EXIT_FAILURE, res);

    EXPECT_EQ(true, parser.get_is_error_request());
}

TEST(RequestParser, ng_request_header_03) {
    std::string content = read_file("ng_request_header_03.txt");
    RequestParser parser;
    int res = parser.parse_request(content);
    EXPECT_EQ(EXIT_FAILURE, res);

    EXPECT_EQ(true, parser.get_is_error_request());
}
