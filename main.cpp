#include <iostream>
#include <chrono>
#include <random>
#include <math.h>
#include <bits/stdc++.h>

#include "my_vector.hpp"
#include "util.hpp"

#define DEBUG 0

using namespace std;
using namespace std::chrono;

float brute_NN(list <my_vector> *data, my_vector query);
void run_brute_force();
unsigned int h(my_vector x, my_vector s, float w=4000, int k=4, unsigned int m=pow(2,32)-5);

int main(){

  //run_brute_force();

  float w=4000;
  int k=4;
  unsigned int m=pow(2,32)-5;

  list <my_vector> *data=read_vector_file("input_small_id");
  list <my_vector> *queries=read_vector_file("query_small_id");
  // list <my_vector> *data=read_vector_file("input_b_id");
  // list <my_vector> *queries=read_vector_file("query_b_id");
  // list <my_vector> *data=read_vector_file("my_input.txt");
  // list <my_vector> *queries=read_vector_file("my_queries.txt");

  default_random_engine generator;
  uniform_real_distribution<double> distribution(0.0,w);//!! den 3ero an mpori na dosi ta akra
  my_vector s(128);

  for(unsigned int i=0;i<s.get_dimentions();i++)
    s.coordinates[i]=distribution(generator);

  for(list <my_vector>::iterator it = data->begin(); it != data->end(); ++it){
    cout<<h(*it,s,w,k,m)<<endl;
  }


  return 0;
}

template <typename T>
T modpow(T base, T exp, T modulus) {
  base %= modulus;
  T result = 1;
  while (exp > 0) {
    if (exp & 1) result = (result * base) % modulus;
    base = (base * base) % modulus;
    exp >>= 1;
  }
  return result;
}

unsigned int h(my_vector x, my_vector s, float w, int k, unsigned int m){
  unsigned int d=x.get_dimentions();
  int a_i=0;
  unsigned int M=pow(2,32/k);
  unsigned int ans=0.0;
  unsigned int a_small,m_small;

  for(unsigned int i=0;i<d;i++){
    a_i=(int)(floor((x.coordinates[i]-s.coordinates[i])/w));
    a_small=a_i%M;
    m_small=modpow(m, d-(i+1), M);
    ans+=(m_small*a_small)%M;
  }

  return ans;
}

void run_brute_force(){
  // #if DEBUG
  // cout<<"Destructing "<<sizeof(coordinates)/sizeof(*coordinates)<<"d vector"<<'\n';
  // #endif

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
    if(ans>tmp)
      ans=tmp;
  }

  return ans;
}
