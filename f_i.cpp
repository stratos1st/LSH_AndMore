#include <iostream>
#include <algorithm>
#include <chrono>

#include "f_i.hpp"

#define DEBUG 0

using namespace std;

f_i::f_i(unsigned int dimentions, unsigned int number_of_train_data, const float _w,
          const unsigned int _k, const unsigned int _m):w(_w),k(_k),
          m(_m),new_d(log2(number_of_train_data)){
  #if DEBUG
  cout<<"Constructing f_i"<<'\n';
  #endif
  using namespace std::chrono;
  auto seed_t=system_clock::now().time_since_epoch();
  auto seed_m=duration_cast<nanoseconds>(seed_t);
  generator.seed(seed_m.count());

  my_g_i=new g_i(dimentions,w,k,m);
}

f_i::~f_i(){
  #if DEBUG
  cout<<"Destructing f_i"<<'\n';
  #endif
  dictionary.clear();
  delete my_g_i;
}

bool f_i::get_f_i(my_vector x){
  unordered_map<long int, bool>::iterator pos;
  if((pos=dictionary.find(my_g_i->get_g_x(x)))!=dictionary.end())
    return pos->second;
  else{
    uniform_int_distribution<short int> distribution(0,1);//!! den 3ero an mpori na dosi ta akra
    bool tmp=distribution(generator);
    dictionary.insert({my_g_i->get_g_x(x),tmp});
    return tmp;
  }
}
