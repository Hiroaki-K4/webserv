# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/12 22:34:53 by hkubo             #+#    #+#              #
#    Updated: 2023/02/05 22:11:53 by hkubo            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := webserv

SRCS := srcs/main.cpp

OBJS := $(SRCS:.cpp=.o)

CXX := c++
CXXFLAGS := -Wall -Wextra -Werror -std=c++98
INCLUDE := -I.

all: $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

$(NAME): $(OBJS)
	$(CXX) $(OBJS) -o $(NAME)

clean:
	rm $(OBJS)

fclean: clean
	rm $(NAME)

re: fclean all

.PHONY: all clean fclean re test
