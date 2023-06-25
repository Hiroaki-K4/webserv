/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_utils.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 16:42:19 by hkubo             #+#    #+#             */
/*   Updated: 2023/06/25 17:00:45 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io_utils.hpp"

void io_init(io_s *io_s, int fd) {
    io_s->io_fd = fd;
    io_s->read_count = 0;
    io_s->io_buf_ptr = io_s->io_buf;
}

ssize_t io_write(int fd, void *input, size_t len) {
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

static int io_read(io_s *io_s, char *read_buf, size_t n) {
    while (io_s->read_count >= 0) {
        std::cout << "io_s->read_count: " << io_s->read_count << std::endl;
        std::cout << "sizeof(io_s->io_buf): " << sizeof(io_s->io_buf) << std::endl;
        io_s->read_count = read(io_s->io_fd, io_s->io_buf, sizeof(io_s->io_buf));
        std::cout << "std::string(io_s->io_buf): " << std::string(io_s->io_buf) << std::endl;
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
            std::cout << "break" << std::endl;
            break;
        }
    }

    int pass_count = n;
    if (io_s->read_count < (int)n) pass_count = io_s->read_count;
    memcpy(read_buf, io_s->io_buf_ptr, pass_count);
    io_s->io_buf_ptr += pass_count;
    io_s->read_count -= pass_count;

    return pass_count;
}

int io_read_line(io_s *io_s, void *read_buf, size_t max_line) {
    size_t n;
    char c, *buf_p = static_cast<char *>(read_buf);
    int read_size = 1;

    int new_line_count = 0;
    for (n = 1; n < max_line; n++) {
        std::cout << "loop_check" << std::endl;
        int read_count = io_read(io_s, &c, read_size);
        std::cout << "loop_check2" << std::endl;
        n += read_count;
        if (read_count == read_size) {
            if (c == '\n') {
                new_line_count += 1;
                if (new_line_count == 2) {
                    std::cout << "break1" << std::endl;
                    break;
                }
            } else if (c == '\r') {
                continue;
            } else {
                new_line_count = 0;
            }
            *buf_p++ = c;
        } else if (read_count == 0) {
            std::cout << "break2" << std::endl;
            break;
        } else {
            std::cout << "break3" << std::endl;
            return FAILURE;
        }
    }
    *buf_p = 0;

    return n - 1;
}

int io_read_request(io_s *io_s) {
    while (io_s->read_count >= 0) {
        io_s->io_buf = (char *)malloc(sizeof(char) * (IO_BUFSIZE + 1));
        std::cout << "loop start" << std::endl;
        io_s->read_count = read(io_s->io_fd, io_s->io_buf, IO_BUFSIZE);
        std::cout << "std::string(io_s->io_buf) length: " << std::string(io_s->io_buf).length() << std::endl;
        std::cout << "io_s->read_count: " << io_s->read_count << std::endl;
        if (io_s->read_count < 0) {
            if (errno != EINTR) {
                std::cout << "[ERROR] io_read: read error" << std::endl;
                return FAILURE;
            }
        } else if (io_s->read_count == 0) {
            std::cout << "[INFO] io_read_request: EOF" << std::endl;
            return SUCCESS;
        } else {
            io_s->io_buf_ptr = io_s->io_buf;
            // std::cout << "break" << std::endl;
            // break;
            io_s->io_request = io_s->io_request + std::string(io_s->io_buf);
            std::cout << "io_request len: " << io_s->io_request.length() << std::endl;
            std::cout << "std::string(io_s->io_buf) len: " << std::string(io_s->io_buf).length() << std::endl;
            if (io_s->read_count < IO_BUFSIZE) {
                std::cout << "[INFO] io_read_request: EOF" << std::endl;
                return SUCCESS;
            }
        }
        free(io_s->io_buf);
        // io_s->io_buf = NULL;
        std::cout << "loop finish1" << std::endl;
    }
    std::cout << "loop finish2" << std::endl;
    std::cout << "io_s->io_request: " << io_s->io_request << std::endl;
    return SUCCESS;
}
