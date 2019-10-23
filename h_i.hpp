#ifndef H_I
#define H_I

#include <math.h>

#include "my_vector.hpp"

class h_i{
  private:
    my_vector *s;
    const unsigned int k,m;
    const float w;
  public:
    h_i(unsigned int dimentions, const float _w=4000,
              const unsigned int _k=4, const unsigned int _m=pow(2,32)-5);
    ~h_i();
    int get_h_x(my_vector &x);//at least 16 bits. ok for k>=2
};

#endif
