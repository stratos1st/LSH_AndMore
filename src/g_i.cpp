#include <iostream>

#include "g_i.hpp"

#define DEBUG 0

using namespace std;

g_i::g_i(unsigned int dimentions, const float _w,
          const unsigned int _k, const unsigned int _m):w(_w),k(_k),m(_m){
  #if DEBUG
  cout<<"Constructing "<<dimentions<<"d g_i"<<'\n';
  #endif

  table_h_i=new h_i*[k];
  for(unsigned int i=0;i<k;i++)
    table_h_i[i]=new h_i(dimentions,w,k,m);
}

g_i::~g_i(){
  #if DEBUG
  cout<<"Destructing "<<k<<"k g_i"<<'\n';
  #endif
  for(unsigned int i=0;i<k;i++)
    delete table_h_i[i];
  delete[] table_h_i;
}

unsigned long int g_i::get_g_x(my_vector &x){
  unsigned long int ans=0;
  for(unsigned int i=0;i<k;i++){
    ans=ans|table_h_i[i]->get_h_x(x);
    ans=ans<<32/k;
  }
  return ans;
}
