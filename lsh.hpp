#ifndef LSH
#define LSH

#include <math.h>
#include <unordered_map>
#include <list>

#include "my_vector.hpp"
#include "g_i.hpp"

class lsh{
protected:
    const float w;
    const unsigned int k,m,l;//g's k,m,w
    g_i **table_g_i;

    lsh(unsigned int dimentions, const unsigned int _l=10, const float _w=4000,
              const unsigned int _k=4, const unsigned int _m=pow(2,32)-5);
    virtual ~lsh();
};

class lsh_vector:lsh{
  private:
    std::unordered_multimap<long int, my_vector*> **hash_table;
    std::list<my_vector> *data;
  public:
    lsh_vector(unsigned int dimentions, const unsigned int _l=10, const float _w=4000,
              const unsigned int _k=4, const size_t _container_sz=9000,
              const unsigned int _m=pow(2,32)-5);//_container_sz is the unordered_multimap initial sz
    ~lsh_vector();
    void train(std::list <my_vector> *train_data_set);
    std::pair<my_vector*, double> find_NN(my_vector &query,
                    double (*distance_metric)(my_vector&, my_vector&));
    std::list<my_vector*>* find_rNN(my_vector &query,  double r,
                    double (*distance_metric)(my_vector&, my_vector&));
};


#endif
