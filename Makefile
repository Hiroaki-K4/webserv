# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/12 22:34:53 by hkubo             #+#    #+#              #
#    Updated: 2023/02/26 22:58:50 by hkubo            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := webserv

SRCS := srcs/main.cpp \
		srcs/rio_utils.cpp \
		srcs/RequestParser.cpp

OBJS := $(SRCS:.cpp=.o)

INCLUDE := -I./includes -I./srcs

CXX := c++
CXXFLAGS := -Wall -Wextra -Werror -std=c++98 $(INCLUDE)

all: $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

format:
	clang-format -i srcs/*.cpp srcs/*.hpp test/*.cpp includes/*.hpp


## Google test
GTEST_DIR   := ./google_test
GTEST := $(GTEST_DIR)/gtest $(GTEST_DIR)/googletest-release-1.11.0

$(GTEST):
	mkdir -p $(GTEST_DIR)
	curl -OL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.tar.gz
	tar -xvzf release-1.11.0.tar.gz googletest-release-1.11.0
	rm -rf release-1.11.0.tar.gz
	python googletest-release-1.11.0/googletest/scripts/fuse_gtest_files.py $(GTEST_DIR)
	mv googletest-release-1.11.0 $(GTEST_DIR)

test: $(GTEST)

.PHONY: all clean fclean re format test
