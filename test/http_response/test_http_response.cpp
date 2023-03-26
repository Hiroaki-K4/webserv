/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_http_response.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 16:30:10 by hkubo             #+#    #+#             */
/*   Updated: 2023/03/26 16:33:37 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>
#include "HttpResponse.hpp"

TEST(HttpResponse, ok_normal_get) {
    EXPECT_EQ(2, 1+1);
}
