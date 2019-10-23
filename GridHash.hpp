#ifndef GRID_HASH
#define GRID_HASH
#include "my_curve.hpp"


class GridHash{
public:
  my_vector* t;   //shift vector
  static double delta;
  GridHash(my_vector& c);   //create manualy a shift in the grid
  GridHash(unsigned int dimentions);
  ~GridHash();
  my_vector* gridify(my_curve& c);//returns gridifed not paded vector
};

#endif
