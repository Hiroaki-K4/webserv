/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_http_response.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 16:30:10 by hkubo             #+#    #+#             */
/*   Updated: 2023/03/27 21:17:37 by hkubo            ###   ########.fr       */
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

TEST(HttpResponse, status_404) {
    std::string content = read_file(TEST_DIR, "status_404.txt");
    RequestParser parser;
    int res = parser.parse_request(content);
    EXPECT_EQ(EXIT_SUCCESS, res);

    HttpResponse resp;
    resp.check_http_request(parser);
    EXPECT_EQ(404, resp.get_http_status());
}
