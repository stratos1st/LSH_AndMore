#ifndef TRAVERSAL_PROJECTION
#define TRAVERSAL_PROJECTION

#include <list>

#include "my_curve.hpp"
#include "lsh.hpp"

class traversal_projection{
  private:
    const unsigned int max_sz;
    std::pair<unsigned int, unsigned int> **all_pairs;
    std::list<std::list<std::pair<unsigned int, unsigned int>*>*> ***big_table;
    std::list<my_curve> *data;
    std::list<my_vector*> data2;//TODO make it * //FIXEME needs deletion myvector*
    std::list<lsh_curve*> **lsh_table;

  public:
    traversal_projection(unsigned int _max_sz=5);
    ~traversal_projection();
    void train(std::list <my_curve> *train_data_set);
    std::pair<my_curve*, double> find_NN(my_curve &query,
                    double (*distance_metric_curve)(my_curve&, my_curve&, double(*distance_metric_vector)(my_vector&, my_vector&))=Dtw,
                    double(*distance_metric_vector)(my_vector&, my_vector&)=manhattan_distance);
    void print_big_table();

};

#endif
