/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rio_utils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 16:42:19 by hkubo             #+#    #+#             */
/*   Updated: 2023/02/11 16:55:10 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void rio_readinitb(rio_t *rp, int fd)
{
    rp->rio_fd = fd;
    rp->rio_cnt = 0;
    rp->rio_bufptr = rp->rio_buf;
}

ssize_t rio_writen(int fd, void *usrbuf, size_t n)
{
    size_t nleft = n;
    ssize_t nwriten;
    char *bufp = static_cast<char *>(usrbuf);

    while (nleft > 0) {
        if ((nwriten = write(fd, bufp, nleft)) <= 0) {
            if (errno == EINTR) // interrupted by a signal while writing no data
                nwriten = 0;
            else
                return -1;
        }
        nleft -= nwriten;
        bufp += nwriten;
    }
    return n;
}

static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n)
{
    int cnt;

    while (rp->rio_cnt <= 0) {
        rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));
        if (rp->rio_cnt < 0) {
            if (errno != EINTR) {
                fprintf(stderr, "read error\n");
                return -1;
            }
        }
        else if (rp->rio_cnt == 0) {
            printf("EOF\n");
            return 0;
        }
        else
            rp->rio_bufptr = rp->rio_buf;
    }

    cnt = n;
    if (rp->rio_cnt < (int)n)
        cnt = rp->rio_cnt;
    memcpy(usrbuf, rp->rio_bufptr, cnt);
    rp->rio_bufptr += cnt;
    rp->rio_cnt -= cnt;
    return cnt;
}

ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen, bool ignore_new_line)
{
    int rc;
    size_t n;
    char c, *bufp = static_cast<char *>(usrbuf);

    for (n = 1; n < maxlen; n++) {
        if ((rc = rio_read(rp, &c, 1)) == 1) {
            *bufp++ = c;
            if (ignore_new_line && c == '\n') {
                n++;
                break;
            }
        } else if (rc == 0) {
            if (n == 1)
                return 0; // EOF, no data read
            else
                break; // EOF, some data was read
        } else {
            return -1; // Error
        }
    }
    *bufp = 0;
    return n - 1;
}
