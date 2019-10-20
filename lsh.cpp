#include <iostream>
#include <bits/stdc++.h>
#include <iterator>
#include <algorithm>

#include "lsh.hpp"
#include "g_i.hpp"
#include "util.hpp"

#define DEBUG 0

using namespace std;

lsh::lsh(unsigned int dimentions, const unsigned int _l, const float _w,
          const unsigned int _k, const unsigned int _m):w(_w),k(_k),m(_m),l(_l){
  #if DEBUG
  cout<<"Constructing lsh"<<'\n';
  #endif
  table_g_i=new g_i*[l];
  for(unsigned int i=0;i<l;i++)
    table_g_i[i]=new g_i(dimentions,w,k,m);
  hash_table=new unordered_multimap<long int, my_vector>*[l];
  for(unsigned int i=0;i<l;i++)
    hash_table[i]=new unordered_multimap<long int, my_vector>;

}

lsh::~lsh(){
  #if DEBUG
  cout<<"Destructing lsh"<<'\n';
  #endif
  for(unsigned int i=0;i<l;i++){
    delete table_g_i[i];
    hash_table[i]->clear();
    delete hash_table[i];
  }
  delete[] table_g_i;
  delete[] hash_table;
}

void lsh::train(list <my_vector> *train_data_set){
  for(unsigned int i=0;i<l;i++)
    for(list <my_vector>::iterator it = train_data_set->begin(); it != train_data_set->end(); ++it)
      hash_table[i]->insert({table_g_i[i]->get_g_x(*it),*it});
}

pair<my_vector*, double> lsh::find_NN(my_vector &query, double (*distance_metric)(my_vector&, my_vector&)){
  my_vector *ans;
  double minn=DBL_MAX;
  for(unsigned int i=0;i<l;i++){
    auto range = hash_table[i]->equal_range(table_g_i[i]->get_g_x(query));
    for(unordered_multimap<long int, my_vector>::iterator it = range.first; it != range.second; ++it){
      double tmp=distance_metric(query, *&it->second);
      if(minn>tmp){
        minn=tmp;
        ans=&it->second;
      }
    }
  }
  return make_pair(ans,minn);
}

list<my_vector*>* lsh::find_rNN(my_vector &query, double r, double (*distance_metric)(my_vector&, my_vector&)){
  list<my_vector*> *ans=new list<my_vector*>;
  for(unsigned int i=0;i<l;i++){
    auto range = hash_table[i]->equal_range(table_g_i[i]->get_g_x(query));
    for(unordered_multimap<long int, my_vector>::iterator it = range.first; it != range.second; ++it){
      double tmp=distance_metric(query, *&it->second);
      if(tmp<=r)
        ans->push_back(&it->second);
    }
  }
  ans->unique();
  return ans;
}
