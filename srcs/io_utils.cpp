/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_utils.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 16:42:19 by hkubo             #+#    #+#             */
/*   Updated: 2023/05/27 16:15:27 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void io_readinitb(io *io_s, int fd) {
    io_s->io_fd = fd;
    io_s->read_count = 0;
    io_s->io_buf_ptr = io_s->io_buf;
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

static int io_read(io *io_s, char *read_buf, size_t n) {
    while (io_s->read_count <= 0) {
        io_s->read_count = read(io_s->io_fd, io_s->io_buf, sizeof(io_s->io_buf));
        if (io_s->read_count < 0) {
            if (errno != EINTR) {
                std::cout << "[ERROR] io_read: read error" << std::endl;
                return FAILURE;
            }
        } else if (io_s->read_count == 0) {
            std::cout << "[INFO] io_read: EOF" << std::endl;
            return 0;
        } else {
            io_s->io_buf_ptr = io_s->io_buf;
        }
    }

    int pass_count = n;
    if (io_s->read_count < (int)n) pass_count = io_s->read_count;
    memcpy(read_buf, io_s->io_buf_ptr, pass_count);
    io_s->io_buf_ptr += pass_count;
    io_s->read_count -= pass_count;

    return pass_count;
}

int io_read_line(io *io_s, void *read_buf, size_t maxlen, bool ignore_new_line) {
    size_t n;
    char c, *buf_p = static_cast<char *>(read_buf);

    for (n = 1; n < maxlen; n++) {
        int read_count = io_read(io_s, &c, 1);
        if (read_count == 1) {
            *buf_p++ = c;
            if (ignore_new_line && c == '\n') {  // Finish read line
                n++;
                break;
            }
        } else if (read_count == 0) {
            if (n == 1)
                return 0;  // EOF, no data read
            else
                break;  // EOF, some data was read
        } else {
            return FAILURE;  // Error
        }
    }
    *buf_p = 0;

    return n - 1;
}
