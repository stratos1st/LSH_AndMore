CC= g++
CGLAG=  -ggdb -lm -g -m64 -O0

main: main.o my_vector.o util.o h_i.o g_i.o lsh.o
	$(CC) $(CFLAG) -o main main.o my_vector.o util.o h_i.o g_i.o lsh.o

main.o: main.cpp
	$(CC) -c main.cpp

my_vector.o: my_vector.cpp my_vector.hpp
	$(CC) -c my_vector.cpp

util.o: util.cpp util.hpp
	$(CC) -c util.cpp

h_i.o: h_i.cpp h_i.hpp
	$(CC) -c h_i.cpp

g_i.o: g_i.cpp g_i.hpp
	$(CC) -c g_i.cpp

lsh.o: lsh.cpp lsh.hpp
	$(CC) -c lsh.cpp

.PHONY: clean
clean:
	rm -f main.o my_vector.o util.o h_i.o g_i.o lsh.o
