#include <iostream>
#include <random>
#include <chrono>

#include "h_i.hpp"
#include "util.hpp"

#define DEBUG 0

using namespace std;

h_i::h_i(unsigned int dimentions, const float _w,
          const unsigned int _k, const unsigned int _m):w(_w),k(_k),m(_m){
  using namespace std::chrono;
  #if DEBUG
  cout<<"Constructing "<<dimentions<<"d h_i"<<'\n';
  #endif
  //seeding generator
  default_random_engine generator;
  auto seed_t=system_clock::now().time_since_epoch();
  auto seed_m=duration_cast<nanoseconds>(seed_t);
  generator.seed(seed_m.count());

  uniform_real_distribution<double> distribution(0.0,w);
  s=new my_vector(dimentions);

  for(unsigned int i=0;i<s->get_dimentions();i++)
    s->coordinates[i]=distribution(generator);
}

h_i::~h_i(){
  #if DEBUG
  cout<<"Destructing "<<s->get_dimentions()<<"d h_i"<<'\n';
  #endif
  delete s;
}

int h_i::get_h_x(my_vector &x){
  if(x.get_dimentions()!=s->get_dimentions()){
    cerr<<"\n\n!! get_h_x dimentions ERROR!!\n\n";
    exit(1);
  }

  unsigned int d=x.get_dimentions();
  int a_i=0;
  unsigned int M=pow(2,32/k);
  int ans=0;
  unsigned int a_small,m_small;

  for(unsigned int i=0;i<d;i++){
    a_i=(int)(floor((x.coordinates[i]-s->coordinates[i])/w));
    a_small=(M + (a_i%M)) % M;//negative values using %
    m_small=modpow(m, d-(i+1), M);//overflowing if not for modpow
    ans+=(m_small*a_small)%M;
  }

  return ans;
}
