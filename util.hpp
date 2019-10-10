#ifndef UTIL
#define UTIL

#include <list>
#include <string>

#include "my_vector.hpp"

int manhattan_distance(my_vector a, my_vector b);
std::list <my_vector>* read_vector_file(std::string name);

#endif
