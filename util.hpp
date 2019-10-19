#ifndef UTIL
#define UTIL

#include <list>
#include <string>

#include "my_curves.hpp"

extern double Dtw( my_curve& x, my_curve& y);
extern double manhattan_distance(my_vector a, my_vector b);
extern std::list <my_vector>* read_vector_file(std::string name);
template <typename T>
extern T modpow(T base, T exp, T modulus);
extern short int hammingDistance(short int n1, short int n2);//not used
extern int* get_hamming_distance_01(int x, unsigned int ans_size);//returns an array[ans_size]
//of binary numbers with hamming distance 0 or 1 to binary number x
extern std::pair<my_vector*,double> brute_NN(std::list <my_vector> *data, my_vector &query);
extern std::pair<my_curve*,double> brute_NN_curve(std::list <my_curve> *data, my_curve &query);
extern std::list <my_curve>* read_curve_file(std::string name);
#endif
