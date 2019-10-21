#include <iostream>
#include <chrono>


#include "my_curves.hpp"
#include "util.hpp"

#define DEBUG 0

using namespace std;

int main(){

  list <my_curve> *data=read_curve_file("./.atomignore/trajectories_dataset");
  //list <my_curve> *queries=read_curve_file("./.atomignore/trajectories_dataset");
  //data->front().get_vector(1).print_vec();
  my_curve querie(data->front());
  std::cout << "done with data" << '\n';

  brute_NN_curve(data, querie);
  data->clear();
  delete data;

  return 0;
}
