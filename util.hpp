#ifndef UTIL
#define UTIL

#include <list>
#include <string>

#include "my_curve.hpp"

extern double manhattan_distance(my_vector& a, my_vector& b);
extern double Dtw( my_curve& x, my_curve& y,
                double(*distance_metric)(my_vector&, my_vector&)=manhattan_distance);
extern std::list <my_vector>* read_vector_file(std::string name);
template <typename T>
extern T modpow(T base, T exp, T modulus);
extern short int hammingDistance(short int n1, short int n2);//not used
//of binary numbers with hamming distance 0 or 1 to binary number x
extern std::pair<my_vector*,double> brute_NN(std::list <my_vector> *data, my_vector &query,
                        double(*distance_metric)(my_vector&, my_vector&));
extern std::pair<my_curve*,double> brute_NN_curve(std::list <my_curve> *data, my_curve &query,
                        double(*distance_metric_curve)(my_curve&, my_curve&, double(*distance_metric)(my_vector&, my_vector&)),
                        double(*distance_metric_vector)(my_vector&, my_vector&)=manhattan_distance);
extern std::list <my_curve>* read_curve_file(std::string name);
#endif
