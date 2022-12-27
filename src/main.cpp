#include <iostream>
#include <chrono>
#include <math.h>
#include <unordered_set>
#include <string>
#include <random>

#include "my_vector.hpp"
#include "my_curve.hpp"
#include "util.hpp"
#include "traversal_projection.hpp"

#define DEBUG 0

using namespace std;

int main(){
  double e = 0.5;
  my_curve A(1,3);
  my_vector B(3);
  //= -d log e/e^2 .
  unsigned int d = 2;
  unsigned int K = -d*log2(e)/(e*e);
  std::cout << K << '\n';
  my_curve* Z = random_array(K,d);
  // for (size_t i = 0; i < 3; i++) {
  //   for (size_t j = 0; j < 3; j++) {
  //     A.vectors[i]->coordinates[j] = 1.0;
  //   }
  // }
  ////check malually 1x3 3x1
  // my_vector B(3);
  // A.vectors[0]->coordinates[0] = 2.0;
  // A.vectors[0]->coordinates[1] = -3.0;
  // A.vectors[0]->coordinates[2] = 4.0;
  //
  // B.coordinates[0]= 2.0;
  // B.coordinates[1]= 4.0;
  // B.coordinates[2]= -3.0;
  for (size_t i = 0; i < Z->numofvectors; i++) {
    Z->get_vector(i).print_vec();
  }
  my_vector* l = multiply(A,B);
  l->print_vec();
  delete Z ;
  return 0;
}
