CC= g++
CGLAG=  -ggdb -lm -g -m64 -O0

all: lsh cube curve_grid_lsh curve_projection_lsh curve_grid_hypercube #main

# main: main.o my_vector.o util.o h_i.o g_i.o lsh.o f_i.o  my_curve.o GridHash.o traversal_projection.o
# 	$(CC) $(CFLAG) -o main main.o my_vector.o util.o h_i.o g_i.o lsh.o f_i.o my_curve.o GridHash.o traversal_projection.o

lsh: lsh_main.o my_vector.o util.o h_i.o g_i.o lsh.o my_curve.o GridHash.o
	$(CC) $(CFLAG) -o lsh lsh_main.o my_vector.o util.o h_i.o g_i.o lsh.o my_curve.o GridHash.o

cube: random_projection_main.o random_projection.o my_vector.o util.o h_i.o g_i.o f_i.o my_curve.o GridHash.o
	$(CC) $(CFLAG) -o cube random_projection_main.o random_projection.o my_vector.o util.o h_i.o g_i.o f_i.o my_curve.o GridHash.o

curve_grid_lsh: lsh_main_curves.o my_vector.o util.o h_i.o g_i.o lsh.o my_curve.o GridHash.o
	$(CC) $(CFLAG) -o curve_grid_lsh lsh_main_curves.o my_vector.o util.o h_i.o g_i.o lsh.o my_curve.o GridHash.o

curve_projection_lsh: traversal_projection_main.o my_vector.o util.o h_i.o g_i.o lsh.o my_curve.o GridHash.o traversal_projection.o
	$(CC) $(CFLAG) -o curve_projection_lsh traversal_projection_main.o my_vector.o util.o h_i.o g_i.o lsh.o my_curve.o GridHash.o traversal_projection.o

curve_grid_hypercube: curve_grid_hypercube_main.o my_vector.o util.o random_projection.o h_i.o g_i.o f_i.o my_curve.o GridHash.o
	$(CC) $(CFLAG) -o curve_grid_hypercube curve_grid_hypercube_main.o my_vector.o util.o random_projection.o h_i.o g_i.o f_i.o my_curve.o GridHash.o

traversal_projection_main.o: traversal_projection_main.cpp
	$(CC) -c traversal_projection_main.cpp

curve_grid_hypercube_main.o: curve_grid_hypercube_main.cpp
	$(CC) -c curve_grid_hypercube_main.cpp

random_projection_main.o: random_projection_main.cpp
	$(CC) -c random_projection_main.cpp

lsh_main.o: lsh_main.cpp
	$(CC) -c lsh_main.cpp

# main.o: main.cpp
# 	$(CC) -c main.cpp

lsh_main_curves.o: lsh_main_curves.cpp
	$(CC) -c lsh_main_curves.cpp

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

traversal_projection.o: traversal_projection.cpp traversal_projection.hpp
	$(CC) -c traversal_projection.cpp

GridHash.o: GridHash.cpp GridHash.hpp
	$(CC) -c GridHash.cpp

.PHONY: clean
clean:
	rm -f curve_grid_hypercube_main.o curve_projection_lsh traversal_projection_main.o traversal_projection.o cube curve_grid_lsh lsh_main_curves.o random_projection_main.o lsh lsh_main.o main main.o my_vector.o util.o h_i.o g_i.o lsh.o f_i.o random_projection.o my_curve.o GridHash.o
