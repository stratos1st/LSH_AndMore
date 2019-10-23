#ifndef TRAVERSAL_PROJECTION
#define TRAVERSAL_PROJECTION

#include <list>

#include "my_curve.hpp"
#include "lsh.hpp"

class traversal_projection{
  private:
    const unsigned int max_sz;
    std::pair<unsigned int,unsigned int> **all_pairs;
    std::list<std::list<std::pair<unsigned int,unsigned int>*>*> ***big_table;
    std::list <my_curve> *data;

  public:
    traversal_projection(unsigned int _max_sz=5);
    ~traversal_projection();
    void train(std::list <my_curve> *train_data_set);
    // std::pair<my_curve*, double> find_NN(my_curve &query,
    //             double(*distance_metric)(my_curve&, my_curve&),
    //             unsigned int max_points, unsigned int prodes);
    void print_big_table();

};

#endif
