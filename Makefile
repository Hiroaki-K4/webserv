# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/12 22:34:53 by hkubo             #+#    #+#              #
#    Updated: 2023/02/08 22:28:07 by hkubo            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := webserv
TEST := client

SRCS := srcs/main.cpp
TEST_SRCS := test/main.cpp

OBJS := $(SRCS:.cpp=.o)
TEST_OBJS := $(TEST_SRCS:.cpp=.o)

CXX := c++
CXXFLAGS := -Wall -Wextra -Werror -std=c++98
INCLUDE := -I./includes

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

.PHONY: all clean fclean re test
