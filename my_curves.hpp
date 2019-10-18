#ifndef MY_CURVES
#define MY_CURVES

#include "my_vector.hpp"

class my_curve{
 public:
  my_vector** vectors;
  unsigned int numofvectors;
  unsigned int vectordimentions;

  my_curve(unsigned int points,unsigned int dimentions);//how many points the curve will have and their dimentions
  ~my_curve();

  my_vector& get_vector(unsigned int);
  unsigned int get_dimentions() const;//returns dimentions
  void print_curve(unsigned int until=0);//prints x1,x1,...,xuntil. if until==0 prints all
};

#endif
