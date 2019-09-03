all:
	g++ -std=c++11 -o multi_heap multi_heap.cpp -Wall -Wextra -lpthread
chall:
	g++ -std=c++11 -o multi_heap multi_heap.cpp -Wall -Wextra -lpthread
	strip multi_heap
