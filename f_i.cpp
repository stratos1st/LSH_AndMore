#include <iostream>
#include <algorithm>
#include <chrono>

#include "f_i.hpp"

#define DEBUG 0

using namespace std;

f_i::f_i(unsigned int dimentions, const float _w,
          const unsigned int _k, const unsigned int _m):w(_w),k(_k),m(_m){
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

unsigned short int f_i::get_f_i(my_vector &x){
  unordered_map<unsigned long int, unsigned short int>::iterator pos;
  if((pos=dictionary.find(my_g_i->get_g_x(x)))!=dictionary.end())
    return pos->second;
  else{
    uniform_int_distribution<unsigned short int> distribution(0,1);
    unsigned short int tmp=distribution(generator);
    dictionary.insert({my_g_i->get_g_x(x),tmp});
    return tmp;
  }
}
