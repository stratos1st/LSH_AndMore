#include <iostream>
#include <chrono>
#include <math.h>
#include <unordered_set>


#include "my_vector.hpp"
#include "my_curve.hpp"
#include "util.hpp"
#include "traversal_projection.hpp"


#define DEBUG 0

using namespace std;

int main(){
  char input_file_data[100]("./.atomignore/trajectories_dataset_cut");
  char input_file_queries[100]("./.atomignore/queriecurve");
  //------------------------------------read input files
  list <my_curve> *data1=read_curve_file(input_file_data);
  list <my_curve> *data=new list <my_curve>;
  for(auto it=data1->begin();it!=data1->end();++it)
    if(it->numofvectors<=5)
      data->push_back(*it);

  list <my_curve> *queries1=read_curve_file(input_file_queries);
  list <my_curve> *queries=new list <my_curve>;
  for(auto it=queries1->begin();it!=queries1->end();++it)
    if(it->numofvectors<=5)
      queries->push_back(*it);
  cout<<"read files done\n";

  traversal_projection *a=new traversal_projection(5);
  a->print_big_table();
  a->train(data);
  cout<<"training done\n";

<<<<<<< HEAD

=======
  
>>>>>>> filippos
  return 0;
}
