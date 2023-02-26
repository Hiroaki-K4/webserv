# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/12 22:34:53 by hkubo             #+#    #+#              #
#    Updated: 2023/02/26 14:43:36 by hkubo            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := webserv
TEST := client

SRCS := srcs/main.cpp \
		srcs/rio_utils.cpp \
		srcs/RequestParser.cpp
TEST_SRCS := test/main.cpp srcs/rio_utils.cpp

OBJS := $(SRCS:.cpp=.o)
TEST_OBJS := $(TEST_SRCS:.cpp=.o)

CXX := c++
CXXFLAGS := -Wall -Wextra -Werror -std=c++98
INCLUDE := -I./includes -I./srcs

all: $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

$(NAME): $(OBJS)
	$(CXX) $(OBJS) -o $(NAME)

test: $(TEST)

$(TEST): $(TEST_OBJS)
	$(CXX) $(TEST_OBJS) -o $(TEST)

clean:
	rm $(OBJS) $(TEST_OBJS)

fclean: clean
	rm $(NAME) $(TEST)

re: fclean all

format:
	clang-format -i srcs/*.cpp srcs/*.hpp test/*.cpp includes/*.hpp

.PHONY: all clean fclean re test format
