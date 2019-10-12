#include <iostream>
#include <chrono>

#include "my_vector.hpp"
#include "util.hpp"
#include "lsh.hpp"
#include "random_projection.hpp"

#define DEBUG 0
#define DIMENTIONS 128

using namespace std;

//TODO % negative
//TODO referances
//TODO hash table init size

int main(){
  // float w=4000;
  // int k=4;
  // unsigned int m=pow(2,32)-5;

  list <my_vector> *data=read_vector_file("./.atomignore/input_small_id");
  list <my_vector> *queries=read_vector_file("./.atomignore/query_small_id");
  // list <my_vector> *data=read_vector_file("./.atomignore/input_b_id");
  // list <my_vector> *queries=read_vector_file("./.atomignore/query_b_id");
  // list <my_vector> *data=read_vector_file("my_input.txt");
  // list <my_vector> *queries=read_vector_file("my_queries.txt");

  // lsh lsh_model(DIMENTIONS);
  // lsh_model.train(data);
  // cout<<" lsh training done!!\n";

  random_projection model;
  model.train(data);
  cout<<"random_projection training done!!\n";

  int i=1;
  for(list <my_vector>::iterator it = queries->begin(); it != queries->end(); ++it)
    cout<<i++<<"\t"<<brute_NN(data, *it)<<"\t"<<model.find_NN(*it)/*<<"\t"<<lsh_model.find_NN(*it)*/<<endl;



    // // #if DEBUG
    // // cout<<"Destructing "<<sizeof(coordinates)/sizeof(*coordinates)<<"d vector"<<'\n';
    // // #endif
    // using namespace std::chrono;
    //
    // int i=1;
    // for(list <my_vector> :: iterator it = queries->begin(); it != queries->end(); ++it){
    //   // Use auto keyword to avoid typing long
    //   // type definitions to get the timepoint
    //   // at this instant use function now()
    //   auto start = high_resolution_clock::now();
    //   //it->print_vec();
    //   cout<<i++<<" distance = "<<brute_NN(data,*it)<<"\t";
    //   auto stop = high_resolution_clock::now();
    //
    //   // Subtract stop and start timepoints and
    //   // cast it to required unit. Predefined units
    //   // are nanoseconds, microseconds, milliseconds,
    //   // seconds, minutes, hours. Use duration_cast() function.
    //   auto duration = duration_cast<milliseconds>(stop - start);
    //
    //   // To get the value of duration use the count()
    //   // member function on the duration object
    //   cout<<"time = "<<duration.count() << " microseconds\n";
    // }

  data->clear();
  queries->clear();
  delete data;
  delete queries;

  return 0;
}
