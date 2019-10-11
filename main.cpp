#include <iostream>
#include <chrono>
#include <random>
#include <math.h>
#include <bits/stdc++.h>

#include "my_vector.hpp"
#include "util.hpp"
#include "lsh.hpp"

#define DEBUG 0
#define DIMENTIONS 128

using namespace std;

float brute_NN(list <my_vector> *data, my_vector query);
void run_brute_force();

int main(){

  //run_brute_force();

  // float w=4000;
  // int k=4;
  // unsigned int m=pow(2,32)-5;

  list <my_vector> *data=read_vector_file("./.atomignore/input_small_id");
  list <my_vector> *queries=read_vector_file("./.atomignore/query_small_id");
  // list <my_vector> *data=read_vector_file("./.atomignore/input_b_id");
  // list <my_vector> *queries=read_vector_file("./.atomignore/query_b_id");
  // list <my_vector> *data=read_vector_file("my_input.txt");
  // list <my_vector> *queries=read_vector_file("my_queries.txt");

  lsh model(DIMENTIONS);

  int i=1;
  model.train(data);
  for(list <my_vector>::iterator it = queries->begin(); it != queries->end(); ++it)
    cout<<i++<<"\t"/*<<brute_NN(data, *it)*/<<"\t"<<model.find_NN(*it)<<endl;

  data->clear();
  queries->clear();
  delete data;
  delete queries;

  return 0;
}

void run_brute_force(){
  // #if DEBUG
  // cout<<"Destructing "<<sizeof(coordinates)/sizeof(*coordinates)<<"d vector"<<'\n';
  // #endif
  using namespace std::chrono;

  list <my_vector> *data=read_vector_file("input_small_id");
  list <my_vector> *queries=read_vector_file("query_small_id");
  // list <my_vector> *data=read_vector_file("input_b_id");
  // list <my_vector> *queries=read_vector_file("query_b_id");
  // list <my_vector> *data=read_vector_file("my_input.txt");
  // list <my_vector> *queries=read_vector_file("my_queries.txt");

  int i=1;
  for(list <my_vector> :: iterator it = queries->begin(); it != queries->end(); ++it){
    // Use auto keyword to avoid typing long
    // type definitions to get the timepoint
    // at this instant use function now()
    auto start = high_resolution_clock::now();
    //it->print_vec();
    cout<<i++<<" distance = "<<brute_NN(data,*it)<<"\t";
    auto stop = high_resolution_clock::now();

    // Subtract stop and start timepoints and
    // cast it to required unit. Predefined units
    // are nanoseconds, microseconds, milliseconds,
    // seconds, minutes, hours. Use duration_cast() function.
    auto duration = duration_cast<milliseconds>(stop - start);

    // To get the value of duration use the count()
    // member function on the duration object
    cout<<"time = "<<duration.count() << " microseconds\n";
  }

  data->clear();
  queries->clear();
  delete data;
  delete queries;
}

float brute_NN(list <my_vector> *data, my_vector query){
  float ans=INT_MAX,tmp;

  for(list <my_vector> :: iterator it = data->begin(); it != data->end(); ++it){
    tmp=manhattan_distance(query, *it);
    ans=min(ans,tmp);
  }

  return ans;
}
