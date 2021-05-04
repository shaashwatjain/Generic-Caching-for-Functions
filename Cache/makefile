CC = g++
DEBUG_VAL=0
CPPFLAGS = -DDEBUG=${DEBUG_VAL} -Wall -Werror -std=c++2a -pedantic -Weffc++ -fstack-protector-all -fstack-clash-protection -g
HEADERS = cache.hpp hash_utility.hpp lfu_cache.hpp lru_cache.hpp mfu_cache.hpp mru_cache.hpp random_cache.hpp test_functions.hpp

all: a.out

a.out: main.cpp ${HEADERS}
	${CC} main.cpp ${CPPFLAGS} 

.PHONY: documentation

documentation:
	doxygen Doxyfile
