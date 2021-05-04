CC = g++
DEBUG_VAL=0
CPPFLAGS = -DDEBUG=${DEBUG_VAL} -Wall -Werror -std=c++2a -pedantic -Weffc++ -g
HEADERS = lru_cache.hpp mru_cache.hpp hash_utility.hpp cache.hpp test_functions.hpp lfu_cache.hpp random_cache.hpp

all: a.out

a.out: main.cpp ${HEADERS}
	${CC} main.cpp ${CPPFLAGS} 

.PHONY: documentation

documentation:
	doxygen Doxyfile
	