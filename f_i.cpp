#include <iostream>
#include <algorithm>
#include <chrono>

#include "f_i.hpp"

#define DEBUG 0

using namespace std;

f_i::f_i(unsigned int dimentions, const float _w,
          const unsigned int _k, size_t container_sz, const unsigned int _m):w(_w),k(_k),m(_m){
  #if DEBUG
  cout<<"Constructing f_i"<<'\n';
  #endif
  //seeding generator
  using namespace std::chrono;
  auto seed_t=system_clock::now().time_since_epoch();
  auto seed_m=duration_cast<nanoseconds>(seed_t);
  generator.seed(seed_m.count());

  dictionary.reserve(container_sz);

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
  unsigned long int tmp=my_g_i->get_g_x(x);
  if((pos=dictionary.find(tmp))!=dictionary.end())//if g(x) has already been assigned 0,1
    return pos->second;
  else{//if g(x) appeares for the first time
    uniform_int_distribution<unsigned short int> distribution(0,1);
    unsigned short int rand=distribution(generator);
    dictionary.insert({tmp,rand});
    return rand;
  }
}
