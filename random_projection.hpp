#ifndef RANDOM_PROJECTION
#define RANDOM_PROJECTION

#include <math.h>
#include <unordered_map>
#include <list>

#include "GridHash.hpp"
#include "my_curve.hpp"
#include "my_vector.hpp"
#include "f_i.hpp"
#include "util.hpp"

extern int next_mask(int prev_mask);
extern unsigned long int* get_search_buckets(unsigned int x, unsigned int prodes, unsigned int new_d);

class random_projection{
  protected:
    const float w;
    const unsigned int k,m,new_d;//g's k,m,w
    const size_t f_container_sz;//f's unordered map initial sz
    f_i **table_f_i;

    unsigned long int hash_function(my_vector &x);//returns concatination of multiple f_i
  public:
    random_projection(const float _w=4000, const unsigned int _k=4, const unsigned int _new_d=3,
              const size_t _container_sz=9000, const size_t _f_container_sz=200,//FIXEME container sz not needed
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

class random_projection_curve:random_projection{
  private:
    const double pad_number;
    const unsigned int max_curve_sz;
    GridHash* gridhashfunctions;
    std::unordered_multimap<long int,std::pair<my_curve*,my_vector*>> *hash_table;
    std::list<my_curve> *data;

    my_vector* gridify_and_padd(my_curve& curve, double pad_value=99999.99999);
  public:
    random_projection_curve(unsigned int _max_curve_sz, const float _w=0.001,
              const unsigned int _k=4, const unsigned int _new_d=4, double _pad_number=99999.99999,
              const size_t _container_sz=9000, const size_t _f_container_sz=200,
              const unsigned int _m=pow(2,32)-5);//_container_sz is the unordered_multimap initial sz
    ~random_projection_curve();
    void train(std::list <my_curve> *train_data_set);
    void train(std::list <std::pair<my_curve*,my_vector*>> *train_data_set);
    std::pair<my_curve*, double> find_NN(my_curve &query,
                    double (*distance_metric_curve)(my_curve&, my_curve&, double(*distance_metric_vector)(my_vector&, my_vector&))=Dtw,
                    double(*distance_metric_vector)(my_vector&, my_vector&)=manhattan_distance);
    std::pair<my_curve*, double> find_NN(std::pair<my_curve*,my_vector*> &query,
                    double (*distance_metric_curve)(my_curve&, my_curve&, double(*distance_metric_vector)(my_vector&, my_vector&))=Dtw,
                    double(*distance_metric_vector)(my_vector&, my_vector&)=manhattan_distance);
    std::list<my_curve*>* find_rNN(my_curve &query,  double r,
                    double (*distance_metric_curve)(my_curve&, my_curve&, double(*distance_metric_vector)(my_vector&, my_vector&))=Dtw,
                    double(*distance_metric_vector)(my_vector&, my_vector&)=manhattan_distance);
};

#endif
