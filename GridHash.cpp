#include <iostream>
#include <list>
#include <stdlib.h>
#include <random>
#include <chrono>
#include <cmath>
#include <tgmath.h>
#include "GridHash.hpp"

using namespace std;

#define DEBUG 0

double GridHash::delta = 0.009;//FIXEME den prepi na ine edo

double dmod(double &x,double &y){
  return x - (int)(x/y)*y;
}

my_vector* GridHash::gridify(my_curve& c){// maybe call it vectorize
  if (c.vectordimentions != t->dim) {
    cerr<<"\n\n!! ERROR this curve has dif dimentions from the shift vec !!\n\n";
    exit(1);
  }
  //snap - map point to nearest grid point
  list <my_vector> gridcurve;//this list will be used later for the merge of same vectors
  for(unsigned int i=0;i<c.numofvectors;i++){
    my_vector newpoint(c.vectordimentions);// make a new copy of the vector that will be saved in the list
    for(unsigned int j=0;j<c.vectordimentions;j++)
    {
      double x=c.vectors[i]->coordinates[j];
      double shift = x-t->coordinates[j]; // i am shifting temporarily allthe points from the opposite direction istead of the curve being shifted
      newpoint.coordinates[j]=floor(shift/delta)*delta+(dmod(shift,delta)>delta/2)*delta+t->coordinates[j];// !!for delta 0.3 ocurs an undifind problem
    }
    gridcurve.push_back(newpoint);
  }
  // remove dublicates
  #if DEBUG
  for(list <my_vector> :: iterator it = gridcurve.begin(); it != gridcurve.end(); ++it)
    it->print_vec();
  #endif
  gridcurve.unique();
  //concate points
  my_vector* vectorcurve = new my_vector(gridcurve.size()*c.vectordimentions);
  unsigned int i = 0;
  for(list <my_vector> :: iterator it = gridcurve.begin(); it != gridcurve.end(); ++it){
    for (unsigned int j = 0; j <= c.vectordimentions-1; j++) {
      vectorcurve->coordinates[i++]=it->coordinates[j];
    }
  }
  #if DEBUG
  for(list <my_vector> :: iterator it = gridcurve.begin(); it != gridcurve.end(); ++it)
    it->print_vec();
  #endif
  return vectorcurve;
}

GridHash::GridHash(unsigned int dimentions){
  //randomly generate t vector
  // Initializing of uniform_real_distribution class
  t=new my_vector(dimentions);
  auto seed_t=chrono::system_clock::now().time_since_epoch();
  auto seed_m=chrono::duration_cast<chrono::nanoseconds>(seed_t);
  default_random_engine generator (seed_m.count());
  uniform_real_distribution<double> distribution(0, delta);
  for (unsigned int i = 0; i < dimentions; i++) {
    double shift = distribution(generator);
    t->coordinates[i]=shift;
  }
}

GridHash::GridHash(my_vector& c){
  for (unsigned int i = 0; i <= c.get_dimentions()-1; i++)
    if (c.coordinates[i]>= delta) {
      cerr<<"\n\n!! ERROR t is not a shift vector !!\n\n";
      exit(1);
  }
  t=new my_vector(c);
}

GridHash::~GridHash(){
  delete t;
}
