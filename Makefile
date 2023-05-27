# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/12 22:34:53 by hkubo             #+#    #+#              #
#    Updated: 2023/05/27 16:25:24 by hkubo            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := webserv

SRCS := srcs/main.cpp \
		srcs/io_utils.cpp \
		srcs/server_utils.cpp \
		srcs/RequestParser.cpp \
		srcs/HttpServer.cpp \
		srcs/HttpResponse.cpp \
		srcs/ServerConfig.cpp \
		srcs/ServerLocation.cpp \
		srcs/ConfigParser.cpp \
		srcs/utils.cpp

OBJS := $(SRCS:%.cpp=%.o)

INCLUDE := -I./srcs

CXX := c++
CXXFLAGS := -Wall -Wextra -Werror -std=c++98 -pthread $(INCLUDE)

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
	clang-format -i srcs/*.cpp srcs/*.hpp test/*.cpp

client: CXXFLAGS := -Wall -Wextra -Werror -std=c++11 -pthread $(INCLUDE)
client: $(filter-out srcs/main.o, $(OBJS)) test/main.o
	$(CXX) $(CXXFLAGS) $(filter-out srcs/main.cpp, $(SRCS)) test/main.cpp -o client

## Google test
GTEST_DIR   := ./google_test
GTEST := $(GTEST_DIR)/gtest $(GTEST_DIR)/googletest-release-1.11.0
GTEST_MAIN  := $(GTEST_DIR)/googletest-release-1.11.0/googletest/src/gtest_main.cc
GTEST_ALL   := $(GTEST_DIR)/gtest/gtest-all.cc

TESTER_NAME := ./tester
TEST_SRCS := test/request_parser/test_request_parser.cpp \
			 test/http_response/test_http_response.cpp \
			 test/config_parser/test_config_parser.cpp
TEST_OBJS := $(filter-out srcs/main.o, $(OBJS)) $(TEST_SRCS:%.cpp=%.o)

$(GTEST):
	mkdir -p $(GTEST_DIR)
	curl -OL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.tar.gz
	tar -xvzf release-1.11.0.tar.gz googletest-release-1.11.0
	rm -rf release-1.11.0.tar.gz
	python googletest-release-1.11.0/googletest/scripts/fuse_gtest_files.py $(GTEST_DIR)
	mv googletest-release-1.11.0 $(GTEST_DIR)

test: CXXFLAGS := --std=c++11 -I$(GTEST_DIR) -g3 -fsanitize=address $(INCLUDE)
test: $(GTEST) $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) $(GTEST_MAIN) $(GTEST_ALL) \
		-lpthread \
		$(TEST_OBJS) \
		-o $(TESTER_NAME)
	$(TESTER_NAME)
	$(RM) srcs/main.o $(TEST_OBJS) $(TESTER_NAME)

cgi:
	$(CXX) -I./contents/cgi contents/cgi/cgi.cpp -o contents/cgi/cgi

.PHONY: all clean fclean re format client test
