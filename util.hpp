#ifndef UTIL
#define UTIL

#include <list>
#include <string>

#include "my_vector.hpp"

extern int manhattan_distance(my_vector a, my_vector b);
extern std::list <my_vector>* read_vector_file(std::string name);
template <typename T>
extern T modpow(T base, T exp, T modulus);
short int hammingDistance(short int n1, short int n2);
int* get_hamming_distance_01(int x, unsigned int ans_size);
int brute_NN(std::list <my_vector> *data, my_vector query);

#endif
