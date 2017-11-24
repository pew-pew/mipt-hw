flags = --std=c++11 -W -Wall -Wextra -pedantic -Werror

all: test_deque

test_deque: test_deque.cpp deque.h
	g++ $(flags) test_deque.cpp -lgtest -o test_deque

run: test_deque
	./test_deque
