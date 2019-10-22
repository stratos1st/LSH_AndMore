#include <iostream>
#include <chrono>


#include "my_curve.hpp"
#include "util.hpp"
#include "lsh.hpp"
#include "random_projection.hpp"
#include "GridHash.hpp"
#define DEBUG 0

using namespace std;

double GridHash::delta = 0.05;

int main(){
//reading test
  //list <my_curve> *data=read_curve_file("./.atomignore/trajectories_dataset");
  //G.t->print_vec();
  // //list <my_curve> *queries=read_curve_file("./.atomignore/trajectories_dataset");
  // //data->front().get_vector(1).print_vec();
  // my_curve querie(data->front());
  // std::cout << "done with data" << '\n';
  //
  // brute_NN_curve(data, querie);
  // data->clear();
  //delete data;


//test for vectorize curve
  // list <my_curve> *data=read_curve_file("./.atomignore/trajectories_dataset");
  // data->pop_front();
  // my_curve testvector = *(data->begin());
  // my_vector zerovector(testvector.vectordimentions);
  // for (size_t i = 0; i < zerovector.dim; i++) {
  //     zerovector.coordinates[i] = 0.02;
  // }
  // GridHash G(zerovector);
  // //G.t->print_vec();
  // my_vector result=G.gridify(testvector);
  // result.print_vec();
  //delete data;
  return 0;
}
