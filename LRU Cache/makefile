CC = g++
CPPFLAGS = -Wall -Werror -std=c++2a -pedantic -Weffc++

all: a.out

a.out: main.cpp lru_cache.hpp lru_utility.hpp
	${CC} $^ ${CPPFLAGS} 