CC = g++
DEBUG_VAL=0
CPPFLAGS = -DDEBUG=${DEBUG_VAL} -Wall -Werror -std=c++2a -pedantic -Weffc++ -g
HEADERS = cache.hpp hash_utility.hpp lfu_cache.hpp lru_cache.hpp mfu_cache.hpp mru_cache.hpp random_cache.hpp test_functions.hpp

all: vector.out scalar.out a.out

a.out: main.cpp ${HEADERS}
	${CC} main.cpp ${CPPFLAGS} 

vector.out: main_vector.cpp ${HEADERS}
	${CC} main_vector.cpp ${CPPFLAGS} -o vector.out

scalar.out: main_scalar.cpp ${HEADERS}
	${CC} main_scalar.cpp ${CPPFLAGS} -o scalar.out

.PHONY: documentation

documentation:
	doxygen Doxyfile
