#!/bin/bash

make format
make
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./webserv 8080
