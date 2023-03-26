/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 13:59:16 by hkubo             #+#    #+#             */
/*   Updated: 2023/03/26 13:59:17 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"

int main(int argc, char *argv[])
{
    char *buf;
    char arg[MAXLINE], content[MAXLINE];

    // Extract the two arguments
    if ((buf = getenv("QUERY_STRING")) != NULL) {
        strcpy(arg, buf);
    }

    // Make the response body
    sprintf(content, "QUERY_STRING=%s", buf);
    sprintf(content, "<h2>Welcome to Hogwarts!!</h2>");
    sprintf(content, "%s<h2>Do you want to join %s?.\r\n</h2>", content, buf);
    sprintf(content, "%s<h1>But you are Satsuma-haaaaaaaaaaan!!\r\n</h1>", content);
    sprintf(content, "%sGood luck!\r\n", content);

    // Generate the HTTP response
    printf("Connection close\r\n");
    printf("Content-length: %d\r\n", (int)strlen(content));
    printf("Content-type: text/html\r\n\r\n");
    printf("%s", content);
    fflush(stdout);

    return 0;
}
