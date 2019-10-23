#ifndef RANDOM_PROJECTION
#define RANDOM_PROJECTION

#include <math.h>
#include <unordered_map>
#include <list>

#include "my_vector.hpp"
#include "f_i.hpp"

class random_projection{
  protected:
    const float w;
    const unsigned int k,m,new_d;//g's k,m,w
    const size_t f_container_sz;//f's unordered map initial sz
    f_i **table_f_i;

    unsigned long int hash_function(my_vector &x);//returns concatination of multiple f_i
  public:
    random_projection(const float _w=4000, const unsigned int _k=4, const unsigned int _new_d=3,
              const size_t _container_sz=9000, const size_t _f_container_sz=200,
              const unsigned int _m=pow(2,32)-5);//_container_sz is the unordered_multimap initial sz
    ~random_projection();
};

class random_projection_vector:random_projection{
  private:
    std::unordered_multimap<unsigned long int, my_vector*> *hash_table;
    std::list<my_vector> *data;
  public:
    random_projection_vector(const float _w=4000, const unsigned int _k=4, const unsigned int _new_d=3,
              const size_t _container_sz=9000, const size_t _f_container_sz=200,
              const unsigned int _m=pow(2,32)-5);//_container_sz is the unordered_multimap initial sz
    ~random_projection_vector();
    void train(std::list <my_vector> *train_data_set);
    std::pair<my_vector*, double> find_NN(my_vector &query,
                double(*distance_metric)(my_vector&, my_vector&),
                unsigned int max_points, unsigned int prodes);
    std::list<my_vector*>* find_rNN(my_vector &query, double r,
                double(*distance_metric)(my_vector&, my_vector&),
                unsigned int max_points, unsigned int prodes);
};


#endif
