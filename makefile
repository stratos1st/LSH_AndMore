CC= g++
CGLAG=  -ggdb -lm -g -m64 -O0

all: main lsh cube

main: main.o my_vector.o util.o h_i.o g_i.o lsh.o f_i.o random_projection.o  my_curve.o
	$(CC) $(CFLAG) -o main main.o my_vector.o util.o h_i.o g_i.o lsh.o f_i.o random_projection.o my_curve.o

lsh: lsh_main.o my_vector.o util.o h_i.o g_i.o lsh.o my_curve.o
	$(CC) $(CFLAG) -o lsh lsh_main.o my_vector.o util.o h_i.o g_i.o lsh.o my_curve.o

cube: random_projection_main.o my_vector.o util.o h_i.o g_i.o f_i.o random_projection.o my_curve.o
	$(CC) $(CFLAG) -o cube random_projection_main.o my_vector.o util.o h_i.o g_i.o f_i.o random_projection.o my_curve.o

random_projection_main.o: random_projection_main.cpp
	$(CC) -c random_projection_main.cpp

lsh_main.o: lsh_main.cpp
	$(CC) -c lsh_main.cpp

main.o: main.cpp
	$(CC) -c main.cpp

my_vector.o: my_vector.cpp my_vector.hpp
	$(CC) -c my_vector.cpp

my_curve.o: my_curve.cpp my_curve.hpp my_vector.hpp
	$(CC) -c my_curve.cpp

util.o: util.cpp util.hpp
	$(CC) -c util.cpp

h_i.o: h_i.cpp h_i.hpp
	$(CC) -c h_i.cpp

g_i.o: g_i.cpp g_i.hpp
	$(CC) -c g_i.cpp

lsh.o: lsh.cpp lsh.hpp
	$(CC) -c lsh.cpp

f_i.o: f_i.cpp f_i.hpp
	$(CC) -c f_i.cpp

random_projection.o: random_projection.cpp random_projection.hpp
	$(CC) -c random_projection.cpp

.PHONY: clean
clean:
	rm -f cube random_projection_main.o lsh lsh_main.o main main.o my_vector.o util.o h_i.o g_i.o lsh.o f_i.o random_projection.o my_curve.o
