#include <iostream>
#include <chrono>
#include <algorithm>
#include <bits/stdc++.h>

#include "random_projection.hpp"
#include "util.hpp"

#define DEBUG 0

using namespace std;

unsigned long int * get_search_buckets(unsigned int x, unsigned int prodes, unsigned int new_d);

random_projection::random_projection(const unsigned int _l, const float _w,//!!! l does not work
          const unsigned int _k, const unsigned int _new_d, const unsigned int _m)
          :w(_w),k(_k),m(_m),l(_l),new_d(_new_d){
  #if DEBUG
  cout<<"Constructing random_projection"<<'\n';
  #endif

  hash_table=new unordered_map<unsigned long int, my_vector>*[l];
  for(unsigned int i=0;i<l;i++)
    hash_table[i]=new unordered_map<unsigned long int, my_vector>;
  table_f_i=NULL;
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
  if(table_f_i!=0){//if training was done
    for(unsigned int i=0;i<l;i++){
      for(unsigned int j=0;j<new_d;j++)
        delete table_f_i[i][j];
      delete[] table_f_i[i];
    }
    delete[] table_f_i;
  }
}

void random_projection::train(list <my_vector> *train_data_set){
  table_f_i=new f_i**[l];
  for(unsigned int i=0;i<l;i++){
    table_f_i[i]=new f_i*[new_d];
    for(unsigned int j=0;j<new_d;j++)
      table_f_i[i][j]=new f_i(train_data_set->front().get_dimentions(),w,k,m);
  }

  for(unsigned int i=0;i<l;i++)
    for(list <my_vector>::iterator it = train_data_set->begin(); it != train_data_set->end(); ++it)
        hash_table[i]->insert({hash_function(*it,i),*it});
}

pair<my_vector*, double> random_projection::find_NN(my_vector &query,
                          double(*distance_metric)(my_vector&, my_vector&),
                          unsigned int max_points, unsigned int prodes){
  my_vector *ans;
  double minn=DBL_MAX;
  for(unsigned int i=0;i<l;i++){//!!! l does not work
    unsigned long int* search_hash_numbers=get_search_buckets(hash_function(query,i),prodes,new_d);
    for(unsigned int j=0;j<prodes;j++){
      auto range = hash_table[i]->equal_range(search_hash_numbers[j]);
      for(unordered_multimap<unsigned long int, my_vector>::iterator it = range.first; it != range.second; ++it){
        double tmp=distance_metric(query, *&it->second);
        if(minn>tmp){
          minn=tmp;
          ans=&it->second;
        }
        max_points--;
        if(max_points==0){
          delete[] search_hash_numbers;
          return make_pair(ans,minn);
        }
      }
    }
    delete[] search_hash_numbers;
  }
  return make_pair(ans,minn);
}

list<my_vector*>* random_projection::find_rNN(my_vector &query, double r,
                    double(*distance_metric)(my_vector&, my_vector&),
                    unsigned int max_points, unsigned int prodes){
  list<my_vector*> *ans=new list<my_vector*>;
  for(unsigned int i=0;i<l;i++){
    unsigned long int* search_hash_numbers=get_search_buckets(hash_function(query,i),prodes,new_d);
    for(unsigned int j=0;j<prodes;j++){
      auto range = hash_table[i]->equal_range(search_hash_numbers[j]);
      for(unordered_multimap<unsigned long int, my_vector>::iterator it = range.first; it != range.second; ++it){
        double tmp=distance_metric(query, *&it->second);
        if(tmp<=r)
          ans->push_back(&it->second);
        max_points--;
        if(max_points==0){
          delete[] search_hash_numbers;
          ans->unique();
          return ans;
        }
      }
    }
    delete[] search_hash_numbers;
  }
  ans->unique();
  return ans;
}

unsigned long int random_projection::hash_function(my_vector &x, unsigned int &iteration){
  unsigned long int ans=0;
  for(unsigned int i=0;i<new_d;i++){
      ans=ans|table_f_i[iteration][i]->get_f_i(x);
      ans=ans<<1;
  }
  return ans;
}

int next_mask(int prev_mask){
  int w; // next permutation of bits

  int t = prev_mask | (prev_mask - 1); // t gets v's least significant 0 bits set to 1
  // Next set to 1 the most significant bit to change,
  // set to 0 the least significant ones, and add the necessary 1 bits.
  w = (t + 1) | (((~t & -~t) - 1) >> (__builtin_ctz(prev_mask) + 1));

  // kani to idio ligo pio arga xoris tin __builtin_ctz
  // unsigned int t = (prev_mask | (prev_mask - 1)) + 1;
  // w = t | ((((t & -t) / (prev_mask & -prev_mask)) >> 1) - 1);

  return w;
}

unsigned long int* get_search_buckets(unsigned int x, unsigned int prodes, unsigned int new_d){
  unsigned long int *ans=new unsigned long int[prodes];
  int initial_mask = 0,mask;
  unsigned int index=1,j=0;

  ans[0]=x;
  while(prodes>index){
    initial_mask=initial_mask<<1;
    initial_mask=initial_mask|1;
    j=0;
    mask=initial_mask;
    while(j<new_d && prodes>index){
      ans[index]=x^mask;
      mask=next_mask(mask);
      j++;
      index++;
    }
  }

  #if DEBUG
  cout<<"----------------------\n";
  unsigned long int n=x;
  while (n) {
    if (n & 1)
        printf("1");
    else
        printf("0");

    n >>= 1;
  }
  cout<<endl;
  for(unsigned int i=0;i<prodes;i++){
    n=ans[i];
    while (n) {
      if (n & 1)
          printf("1");
      else
          printf("0");

      n >>= 1;
    }
    cout<<endl;
  }
  #endif

  return ans;
}
