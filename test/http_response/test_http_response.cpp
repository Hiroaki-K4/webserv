/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_http_response.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 16:30:10 by hkubo             #+#    #+#             */
/*   Updated: 2023/04/01 21:39:46 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>
#include <fstream>
#include "HttpResponse.hpp"
#include "utils.hpp"

const std::string TEST_DIR = "test/http_response/cases/";

TEST(HttpResponse, status_200) {
    std::string content = read_file(TEST_DIR, "status_200.txt");
    RequestParser parser;
    int res = parser.parse_request(content);
    EXPECT_EQ(EXIT_SUCCESS, res);

    HttpResponse resp;
    resp.check_http_request(parser);
    EXPECT_EQ(200, resp.get_http_status());
}

TEST(HttpResponse, status_403) {
    std::string content = read_file(TEST_DIR, "status_403.txt");
    RequestParser parser;
    int res = parser.parse_request(content);
    EXPECT_EQ(EXIT_SUCCESS, res);

    HttpResponse resp;
    resp.check_http_request(parser);
    EXPECT_EQ(200, resp.get_http_status());
}

TEST(HttpResponse, status_404) {
    std::string content = read_file(TEST_DIR, "status_404.txt");
    RequestParser parser;
    int res = parser.parse_request(content);
    EXPECT_EQ(EXIT_SUCCESS, res);

    HttpResponse resp;
    resp.check_http_request(parser);
    EXPECT_EQ(404, resp.get_http_status());
}

TEST(HttpResponse, status_411) {
    std::string content = read_file(TEST_DIR, "status_411.txt");
    RequestParser parser;
    int res = parser.parse_request(content);
    EXPECT_EQ(EXIT_FAILURE, res);

    HttpResponse resp;
    resp.check_http_request(parser);
    EXPECT_EQ(411, resp.get_http_status());
}

TEST(HttpResponse, status_501) {
    std::string content = read_file(TEST_DIR, "status_501.txt");
    RequestParser parser;
    int res = parser.parse_request(content);
    EXPECT_EQ(EXIT_FAILURE, res);

    HttpResponse resp;
    resp.check_http_request(parser);
    EXPECT_EQ(501, resp.get_http_status());
}

TEST(HttpResponse, status_505) {
    std::string content = read_file(TEST_DIR, "status_505.txt");
    RequestParser parser;
    int res = parser.parse_request(content);
    EXPECT_EQ(EXIT_FAILURE, res);

    HttpResponse resp;
    resp.check_http_request(parser);
    EXPECT_EQ(505, resp.get_http_status());
}
