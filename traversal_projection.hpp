#ifndef TRAVERSAL_PROJECTION
#define TRAVERSAL_PROJECTION

#include <list>

#include "my_curve.hpp"
#include "lsh.hpp"
#include "random_projection.hpp"

template<class T>
class traversal_projection{
  private:
    double pad_number;
    const unsigned int max_sz;
    std::pair<unsigned int, unsigned int> **all_pairs;
    std::list<std::list<std::pair<unsigned int, unsigned int>*>*> ***big_table;
    std::list<my_curve> *data;
    std::list<T*> **lsh_table;

  public:
    traversal_projection(unsigned int _max_sz=5);
    ~traversal_projection();
    void train_lsh(std::list <my_curve> *train_data_set,
                            const unsigned int _l=5, const float _w=0.001,
                            const unsigned int _k=4, const size_t _container_sz=100,
                            const double _pad_number=9999.9999,
                            const unsigned int _m=pow(2,32)-5);

    void train_cube(std::list <my_curve> *train_data_set, const float _w=0.001,
                            const unsigned int _k=4, const unsigned int _new_d=3,
                            const double _pad_number=9999.9999,const size_t _container_sz=100,
                            const size_t _f_container_sz=100,
                            const unsigned int _m=pow(2,32)-5);
    std::pair<my_curve*, double> find_NN(my_curve &query, unsigned int window=1,
                    double (*distance_metric_curve)(my_curve&, my_curve&, double(*distance_metric_vector)(my_vector&, my_vector&))=Dtw,
                    double(*distance_metric_vector)(my_vector&, my_vector&)=manhattan_distance);
    void print_big_table();

};

#endif
