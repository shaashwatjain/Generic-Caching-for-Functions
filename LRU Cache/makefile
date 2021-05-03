CC = g++
DEBUG_VAL=1
CPPFLAGS = -DDEBUG=${DEBUG_VAL} -Wall -Werror -Ofast -std=c++2a -pedantic -Weffc++ -g
HEADERS = lru_cache.hpp mru_cache.hpp hash_utility.hpp

all: a.out

a.out: main.cpp ${HEADERS}
	${CC} main.cpp ${CPPFLAGS} 