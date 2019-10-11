#ifndef UTIL
#define UTIL

#include <list>
#include <string>

#include "my_vector.hpp"

extern int manhattan_distance(my_vector a, my_vector b);
extern std::list <my_vector>* read_vector_file(std::string name);
template <typename T>
extern T modpow(T base, T exp, T modulus);

#endif
