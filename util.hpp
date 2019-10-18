#ifndef UTIL
#define UTIL

#include <list>
#include <string>

#include "my_vector.hpp"
#include "my_curves.hpp"

<<<<<<< HEAD
float Dtw( my_curve& x, my_curve& y);
extern int manhattan_distance(my_vector a, my_vector b);
=======
extern int manhattan_distance(my_vector &a, my_vector &b);
>>>>>>> master
extern std::list <my_vector>* read_vector_file(std::string name);
template <typename T>
extern T modpow(T base, T exp, T modulus);
short int hammingDistance(short int n1, short int n2);//not used
int* get_hamming_distance_01(int x, unsigned int ans_size);//returns an array[ans_size]
//of binary numbers with hamming distance 0 or 1 to binary number x
std::pair<my_vector*,int> brute_NN(std::list <my_vector> *data, my_vector &query);

#endif
