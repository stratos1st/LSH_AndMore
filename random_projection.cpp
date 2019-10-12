#include <iostream>
#include <chrono>
#include <algorithm>
#include <bits/stdc++.h>

#include "random_projection.hpp"
#include "util.hpp"

#define DEBUG 0

using namespace std;

random_projection::random_projection(const unsigned int _l, const float _w,
          const unsigned int _k, const unsigned int _m):w(_w),k(_k),m(_m),l(_l){
  #if DEBUG
  cout<<"Constructing random_projection"<<'\n';
  #endif

  hash_table=new unordered_map<int, my_vector>*[l];
  for(unsigned int i=0;i<l;i++)
    hash_table[i]=new unordered_map<int, my_vector>;

  new_d=0;
}

random_projection::~random_projection(){
  #if DEBUG
  cout<<"Destructing random_projection"<<'\n';
  #endif
  for(unsigned int i=0;i<l;i++){
    hash_table[i]->clear();
    delete hash_table[i];
  }
  delete[] hash_table;
  if(new_d!=0){
    for(unsigned int i=0;i<l;i++){
      for(unsigned int j=0;j<new_d;j++)
        delete table_f_i[i][j];
      delete[] table_f_i[i];
    }
    delete[] table_f_i;
  }
}

void random_projection::train(list <my_vector> *train_data_set){
  new_d=log2(train_data_set->size());

  table_f_i=new f_i**[l];
  for(unsigned int i=0;i<l;i++){
    table_f_i[i]=new f_i*[new_d];
    for(unsigned int j=0;j<new_d;j++)
      table_f_i[i][j]=new f_i(train_data_set->front().get_dimentions(),train_data_set->size(),w,k,m);
  }

  for(unsigned int i=0;i<l;i++)
    for(list <my_vector>::iterator it = train_data_set->begin(); it != train_data_set->end(); ++it)
        hash_table[i]->insert({hash_function(*it,i),*it});
}

int random_projection::find_NN(my_vector query){
  int ans=INT_MAX,cnt=0;
  for(unsigned int i=0;i<l;i++){
    int* search_hash_numbers=get_hamming_distance_01(hash_function(query,i),new_d+1);
    for(unsigned int j=0;j<new_d+1;j++){
      auto range = hash_table[i]->equal_range(search_hash_numbers[j]);
      for(unordered_multimap<int, my_vector>::iterator it = range.first; it != range.second; ++it)
        {ans=min(ans,manhattan_distance(query, it->second));cnt++;}
    }
    delete[] search_hash_numbers;
  }
  cout<<" cnt= "<<cnt<<"\t";
  return ans;
}

void random_projection::find_rNN(my_vector query){

}

int random_projection::hash_function(my_vector x, unsigned int iteration){
  int ans=0;
  for(unsigned int i=0;i<new_d;i++){
      ans=ans^table_f_i[iteration][i]->get_f_i(x);
      ans=ans<<1;
  }
  return ans;
}
