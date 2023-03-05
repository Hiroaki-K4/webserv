/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_request_parser.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 08:18:00 by hkubo             #+#    #+#             */
/*   Updated: 2023/03/05 17:04:52 by hkubo            ###   ########.fr       */
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
    std::cout << "req: " << parser.get_request() << std::endl;

    EXPECT_EQ(false, parser.get_is_error_request());
    // Check request line
    EXPECT_EQ(parser.GET, parser.get_request_method());
    EXPECT_EQ("/", parser.get_target_uri());
    EXPECT_EQ(parser.HTTP_VERSION, parser.get_http_version());
}
