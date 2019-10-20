#ifndef LSH
#define LSH

#include <math.h>
#include <unordered_map>
#include <list>

#include "my_vector.hpp"
#include "g_i.hpp"

class lsh{
  private:
    const float w;
    const unsigned int k,m,l;
    g_i **table_g_i;
    std::unordered_multimap<long int, my_vector> **hash_table;
  public:
    lsh(unsigned int dimentions, const unsigned int _l=10, const float _w=4000,
              const unsigned int _k=4, const unsigned int _m=pow(2,32)-5);
    ~lsh();
    void train(std::list <my_vector> *train_data_set);
    std::pair<my_vector*, double> find_NN(my_vector &query,double (*distance_metric)(my_vector&, my_vector&));
    std::list<my_vector*>* find_rNN(my_vector &query,  double r, double (*distance_metric)(my_vector&, my_vector&));
};


#endif
