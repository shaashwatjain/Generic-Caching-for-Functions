CC = g++
DEBUG_VAL=1
CPPFLAGS = -DDEBUG=${DEBUG_VAL} -Wall -Werror -Ofast -std=c++2a -pedantic -Weffc++ -g

all: a.out

a.out: main.cpp lru_cache.hpp lru_utility.hpp
	${CC} main.cpp ${CPPFLAGS} 