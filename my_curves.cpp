#include "my_curves.hpp"

#include <iostream>

using namespace std;

#define DEBUG 0

my_curve::my_curve(unsigned int points,unsigned int dimentions){
  #if DEBUG
  cout<<"Constructing "<<<<dimentions<<"d curve with "<<points<<" points"<<'\n';
  #endif
  // initializing local variables
  numofvectors=points;
  vectordimentions=dimentions;
  //making an empty array of (pointers to ) vectors
  vectors = new my_vector* [numofvectors];
  for(unsigned int i=0;i<=numofvectors-1;i++)
    vectors[i]=new my_vector(vectordimentions);
}

my_curve::~my_curve(){
  #if DEBUG
  cout<<"Destructing "<<dimentions<<"d curve with "<<numofvectors<<" points"<<'\n';
  #endif
  //deleting first each vector and then the whole array
  for(unsigned int i = 0; i<=numofvectors-1;i++)
    delete vectors[i];
  delete[] vectors;
}

my_vector& my_curve::get_vector(unsigned int index){
  return *vectors[index];
}
