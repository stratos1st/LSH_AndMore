#include <iostream>
#include <chrono>


#include "my_curves.hpp"
#include "util.hpp"
#include "lsh.hpp"
#include "random_projection.hpp"

#define DEBUG 0

using namespace std;

//TODO % negative
//TODO referances
//TODO hash table init size

int main(){

  list <my_curve> *data=read_curve_file("./.atomignore/trajectories_dataset");
  //list <my_vector> *queries=read_vector_file("./.atomignore/query_b_id");
  data->clear();
  delete data;

  return 0;
}
