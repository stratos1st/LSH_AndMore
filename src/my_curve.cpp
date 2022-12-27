#include "my_curve.hpp"

#include <iostream>

using namespace std;

#define DEBUG 0

my_curve::my_curve(unsigned int points,unsigned int dimentions){
  #if DEBUG
  cout<<"Constructing "<<dimentions<<"d curve with "<<points<<" points"<<'\n';
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
  cout<<"Destructing "<<vectordimentions<<"d curve with "<<numofvectors<<" points"<<'\n';
  #endif
  //deleting first each vector and then the whole array
  for(unsigned int i = 0; i<=numofvectors-1;i++)
    delete vectors[i];
  delete[] vectors;
}

my_vector& my_curve::get_vector(unsigned int index){
  return *vectors[index];
}

my_curve& my_curve::operator=(const my_curve &other){
  if(numofvectors!=other.numofvectors){
    cerr<<"\n\n!! ERROR my_curve coppy construstor !!\n\n";
    exit(1);
  }
  #if DEBUG
    cout<<"assinging values with = opperator id curve"<<other.id<<'\n';
  #endif

  for(unsigned int i=0;i<numofvectors;i++)
    *vectors[i]=*other.vectors[i];  //this calls vector '=' operator
  id=other.id;

  vectordimentions=other.vectordimentions;
  return *this;
}

my_curve::my_curve(const my_curve &p2){
  #if DEBUG
  cout<<"Copy Constructing "<<p2.numofvectors<<" points curve in("<<p2.vectordimentions<<"d)"<<'\n';
  #endif
  // this creates an empty space for the newcurve that will be filled with the '='operator
  vectordimentions=p2.vectordimentions;
  numofvectors=p2.numofvectors;
  vectors = new my_vector* [numofvectors];
  for(unsigned int i=0;i<=numofvectors-1;i++)
    vectors[i]=new my_vector(vectordimentions);
  *this=p2;   //this calls curve '='operator
}
