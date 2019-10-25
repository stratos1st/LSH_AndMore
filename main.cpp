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

my_vector* multiply(my_curve& G,my_vector& U){//my curve is an array of its points
//check of matrix copatipility
  if(G.vectordimentions != U.dim){
    cout<<"\n\n!! ERROR multiply G*U fault !!\n\n";
    exit(1);
  }
  //initializing the result of the multiplication
  my_vector* result = new my_vector(G.numofvectors);
  result->id = 77777;   // this id is for debug
  // multiplication
  for (size_t i = 0; i < G.numofvectors; i++) {
    for (size_t j = 0; j < G.vectordimentions; j++) {
      result->coordinates[i] +=  U.coordinates[j]*G.vectors[i]->coordinates[j];
    }
  }
  return result;
}

my_curve* random_array(unsigned int k,unsigned int d){
  auto seed_t=chrono::system_clock::now().time_since_epoch();
  auto seed_m=chrono::duration_cast<chrono::nanoseconds>(seed_t);
  std::default_random_engine generator (seed_m.count());
  std::normal_distribution<double> distribution (0.0,1.0);
  my_curve* new_array = new my_curve(k,d);
  new_array->id=99999;// for debug
  //bulid new_array
  for (size_t i = 0; i < k; i++) {
    for (size_t j = 0; j < d; j++) {
      new_array->vectors[i]->coordinates[j] = distribution(generator);
    }
  }
  return new_array;
}

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
