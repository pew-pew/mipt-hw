FLAGS = --std=c++11 -W -Wall -Wextra -pedantic

all: run

test_heap: test_heap.cpp heap.h stupid_heap.h binomial_heap.h leftist_heap.h
	g++ $(FLAGS) test_heap.cpp -lgtest -o test_heap

run: test_heap
	./test_heap
