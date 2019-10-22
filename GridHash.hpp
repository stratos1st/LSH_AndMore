#include <iostream>
#include <list>
#include <stdlib.h>
#include <random>
#include <chrono>
#include <cmath>
#include <tgmath.h>
#include "my_curve.hpp"


class GridHash{
  static double delta;
  my_vector* t;   //shift vector
public:
  GridHash(my_vector& c);   //create manualy a shift in the grid
  GridHash(unsigned int dimentions);
  ~GridHash();
  my_vector& gridify(my_curve& c);
};
