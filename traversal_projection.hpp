#ifndef TRAVERSAL_PROJECTION
#define TRAVERSAL_PROJECTION

#include <list>
#include <unordered_set>

#include "my_curve.hpp"
#include "lsh.hpp"
#include "random_projection.hpp"

#ifdef BOOST_FUNCTIONAL_HASH_HASH_HPP
  #include <boost/functional/hash.hpp>
  typedef std::unordered_set<std::pair<unsigned int,unsigned int>,boost::hash<std::pair<unsigned int,unsigned int>>> my_unordered_set;
#else//!!!! risky messing with namespace std
  namespace std{
    template <> struct hash<std::pair<unsigned int, unsigned int>> {
        inline size_t operator()(const pair<int, int> &v) const {
            hash<unsigned int> uint_hasher;
            return uint_hasher(v.first) ^ uint_hasher(v.second);
        }
    };
  }
  typedef std::unordered_set<std::pair<unsigned int,unsigned int>> my_unordered_set;
#endif

class traversal_projection{
  protected:
    const unsigned int max_sz;
    my_curve  *G;
    double pad_number;
    std::pair<unsigned int, unsigned int> **all_pairs;
    std::list<std::list<std::pair<unsigned int, unsigned int>*>*> ***big_table;
    std::list<my_curve> *data;

    std::pair<my_curve*,my_vector*> project_traversal_to_vector(my_curve* curve,
                      std::list<std::pair<unsigned int,unsigned int>*>* traversal, bool is_query=false);
  public:
    traversal_projection(unsigned int _max_sz=5, unsigned int how_manny_plus=1);
    virtual ~traversal_projection();
    void train(unsigned int dimentions, double e=0.5, double _pad_number=9999.9999);
    void print_big_table();
};

class traversal_projection_lsh:traversal_projection{
  private:
    std::list<lsh_curve*> **lsh_table;

  public:
    traversal_projection_lsh(unsigned int _max_sz=5, unsigned int how_manny_plus=1);
    ~traversal_projection_lsh();
    void train(std::list <my_curve> *train_data_set, double e=0.5,
                            const unsigned int _l=5, const float _w=0.001,
                            const unsigned int _k=4, const size_t _container_sz=100,
                            const double _pad_number=9999.9999,
                            const unsigned int _m=pow(2,32)-5);
    std::pair<my_curve*, double> find_NN(my_curve &query, unsigned int window=1,
                    double (*distance_metric_curve)(my_curve&, my_curve&, double(*distance_metric_vector)(my_vector&, my_vector&))=Dtw,
                    double(*distance_metric_vector)(my_vector&, my_vector&)=manhattan_distance);
};

class traversal_projection_cube:traversal_projection{
  private:
    std::list<random_projection_curve*> **cube_table;

  public:
    traversal_projection_cube(unsigned int _max_sz=5, unsigned int how_manny_plus=1);
    ~traversal_projection_cube();
    void train(std::list <my_curve> *train_data_set, double e=0.5, const float _w=0.001,
                            const unsigned int _k=4, const unsigned int _new_d=3,
                            const double _pad_number=9999.9999,const size_t _container_sz=100,
                            const size_t _f_container_sz=100,
                            const unsigned int _m=pow(2,32)-5);
    std::pair<my_curve*, double> find_NN(my_curve &query, unsigned int max_point, unsigned int prodes, unsigned int window=1,
                    double (*distance_metric_curve)(my_curve&, my_curve&, double(*distance_metric_vector)(my_vector&, my_vector&))=Dtw,
                    double(*distance_metric_vector)(my_vector&, my_vector&)=manhattan_distance);
};

extern my_unordered_set *get_diagonal(unsigned int x_sz, unsigned int y_sz);

extern my_unordered_set *get_diagonal_plus(my_unordered_set *traversals_diagonal,unsigned int y_sz, unsigned int how_manny_plus=1);

extern void make_relevant_traversals(unsigned int i, unsigned  int j, unsigned int m, unsigned int n,
                      std::pair<unsigned int,unsigned int> **path, int pi,
                      my_unordered_set *relevant_squares,
                      std::list<std::list<std::pair<unsigned int,unsigned int>*>*> *relevant_traversals,
                      std::pair<unsigned int,unsigned int>** all_pairs);

extern std::list<std::list<std::pair<unsigned int,unsigned int>*>*> *get_relevant_traversals(unsigned int m, unsigned int n,
                                      std::pair<unsigned int,unsigned int> **all_pairs, unsigned int how_manny_plus=1);
#endif
