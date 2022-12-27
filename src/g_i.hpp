#ifndef G_I
#define G_I

#include <math.h>

#include "my_vector.hpp"
#include "h_i.hpp"

class g_i{
  private:
    h_i **table_h_i;
    const unsigned int k,m;//h's k,m,w
    const float w;
  public:
    g_i(unsigned int dimentions, const float _w=4000,
              const unsigned int _k=4, const unsigned int _m=pow(2,32)-5);
    ~g_i();
    unsigned long int get_g_x(my_vector &x);//at least 32bit
};


#endif
