CC= g++
CGLAG=  -ggdb -Wall -lm -g -m64 -O0

main: main.o my_vector.o util.o
	$(CC) $(CFLAG) -o main main.o my_vector.o util.o

main.o: main.cpp
	$(CC) -c main.cpp

my_vector.o: my_vector.cpp my_vector.hpp
	$(CC) -c my_vector.cpp

util.o: util.cpp util.hpp
	$(CC) -c util.cpp

.PHONY: clean
clean:
	rm -f main.o my_vector.o util.o
