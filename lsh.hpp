#ifndef LSH
#define LSH

#include <math.h>
#include <unordered_map>
#include <list>

#include "GridHash.hpp"
#include "my_curve.hpp"
#include "my_vector.hpp"
#include "g_i.hpp"
#include "util.hpp"

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

class lsh_curve:lsh{
  private:
    const double pad_number;
    const unsigned int max_curve_sz;
    bool trained;
    GridHash** gridhashfunctions;
    std::unordered_multimap<long int,std::pair<my_curve*,my_vector*>> **hash_table;
    std::list<my_curve> *data;

    my_vector* gridify_and_padd(my_curve& curve, unsigned int iteration, double pad_value=999999.999999);
  public:
    lsh_curve(unsigned int vector_dimentions, unsigned int _max_curve_sz, const unsigned int _l=10, const float _w=4000,
              const unsigned int _k=4, const double _pad_number=9999.9999, const size_t _container_sz=90,
              const unsigned int _m=pow(2,32)-5);//_container_sz is the unordered_multimap initial sz
    ~lsh_curve();
    void train(std::list <my_curve> *train_data_set);
    void train(std::list <std::pair<my_curve*,my_vector*>> *train_data_set);
    std::pair<my_curve*, double> find_NN(my_curve &query,
                    double (*distance_metric_curve)(my_curve&, my_curve&, double(*distance_metric_vector)(my_vector&, my_vector&))=Dtw,
                    double(*distance_metric_vector)(my_vector&, my_vector&)=manhattan_distance);
    std::pair<my_curve*, double> find_NN(std::pair<my_curve*,my_vector*> &query,
                    double (*distance_metric_curve)(my_curve&, my_curve&, double(*distance_metric_vector)(my_vector&, my_vector&))=Dtw,
                    double(*distance_metric_vector)(my_vector&, my_vector&)=manhattan_distance);

};

#endif
