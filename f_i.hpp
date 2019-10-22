#ifndef F_I
#define F_I

#include <unordered_map>
#include <random>
#include <math.h>

#include "my_vector.hpp"
#include "g_i.hpp"

class f_i{//aka f_i(g_i(x)).
  private:
    const float w;
    const unsigned int k,m;//g's k,m,w
    std::default_random_engine generator;
    std::unordered_map<unsigned long int, unsigned short int> dictionary;
    //holdes f(x) values and their coresponding 0,1 value
    g_i *my_g_i;
  public:
    f_i(unsigned int dimentions, const float _w=4000,
              const unsigned int _k=4, size_t container_sz=200, const unsigned int _m=pow(2,32)-5);
              //container_sz is the unorderd map initial sz
    ~f_i();
    unsigned short int get_f_i(my_vector &x);//one bit
};


#endif
