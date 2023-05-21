/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_utils.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 16:42:19 by hkubo             #+#    #+#             */
/*   Updated: 2023/05/21 21:30:22 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void io_readinitb(io *io_s, int fd) {
    io_s->io_fd = fd;
    io_s->io_cnt = 0;
    io_s->io_bufptr = io_s->io_buf;
}

ssize_t io_writen(int fd, void *input, size_t len) {
    size_t left_word_len = len;
    ssize_t writen_len;
    char *buf_p = static_cast<char *>(input);

    while (left_word_len > 0) {
        if ((writen_len = write(fd, buf_p, left_word_len)) <= 0) {
            if (errno == EINTR)  // interrupted by a signal while writing no data
                writen_len = 0;
            else
                return FAILURE;
        }
        left_word_len -= writen_len;
        buf_p += writen_len;
    }

    return len;
}

// TODO: Refactor io_read
static ssize_t io_read(io *rp, char *usrbuf, size_t n) {
    int cnt;

    while (rp->io_cnt <= 0) {
        std::cout << "ok" << std::endl;
        rp->io_cnt = read(rp->io_fd, rp->io_buf, sizeof(rp->io_buf));
        if (rp->io_cnt < 0) {
            if (errno != EINTR) {
                return FAILURE;
            }
        } else if (rp->io_cnt == 0) {
            std::cout << "EOF" << std::endl;
            return 0;
        } else {
            rp->io_bufptr = rp->io_buf;
        }
    }

    cnt = n;
    if (rp->io_cnt < (int)n) cnt = rp->io_cnt;
    memcpy(usrbuf, rp->io_bufptr, cnt);
    rp->io_bufptr += cnt;
    rp->io_cnt -= cnt;

    return cnt;
}

ssize_t io_readlineb(io *rp, void *usrbuf, size_t maxlen, bool ignore_new_line) {
    int rc;
    size_t n;
    char c, *bufp = static_cast<char *>(usrbuf);

    for (n = 1; n < maxlen; n++) {
        if ((rc = io_read(rp, &c, 1)) == 1) {
            *bufp++ = c;
            if (ignore_new_line && c == '\n') {
                n++;
                break;
            }
        } else if (rc == 0) {
            if (n == 1)
                return 0;  // EOF, no data read
            else
                break;  // EOF, some data was read
        } else {
            return FAILURE;  // Error
        }
    }
    *bufp = 0;

    return n - 1;
}
