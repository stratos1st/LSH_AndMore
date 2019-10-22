#include <iostream>
#include <chrono>


#include "my_curve.hpp"
#include "util.hpp"
#include "lsh.hpp"
#include "random_projection.hpp"
#include "GridHash.hpp"
#define DEBUG 0

using namespace std;

//TODO % negative
//TODO referances
//TODO hash table init size

int main(){
double x=1.3;
double y=0.3;
std::cout <<x/y<< '\n';
  // list <my_curve> *data=read_curve_file("./.atomignore/trajectories_dataset");
  // //list <my_curve> *queries=read_curve_file("./.atomignore/trajectories_dataset");
  // //data->front().get_vector(1).print_vec();
  // my_curve querie(data->front());
  // std::cout << "done with data" << '\n';
  //
  // brute_NN_curve(data, querie);
  // data->clear();
  // delete data;

  return 0;
}
