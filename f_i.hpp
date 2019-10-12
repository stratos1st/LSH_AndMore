#ifndef F_I
#define F_I

#include <unordered_map>
#include <random>
#include <math.h>

#include "my_vector.hpp"
#include "g_i.hpp"

class f_i{
  private:
    const unsigned int k,m,new_d;
    const float w;
    std::default_random_engine generator;
    std::unordered_map<long int, bool> dictionary;
    g_i *my_g_i;
  public:
    f_i(unsigned int dimentions, unsigned int number_of_train_data, const float _w=4000,
              const unsigned int _k=4, const unsigned int _m=pow(2,32)-5);
    ~f_i();
    bool get_f_i(my_vector x);
};


#endif
