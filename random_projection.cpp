#include <iostream>
#include <chrono>
#include <algorithm>
#include <bits/stdc++.h>

#include "random_projection.hpp"
#include "util.hpp"

#define DEBUG 0

using namespace std;

unsigned long int * get_search_buckets(unsigned int x, unsigned int prodes, unsigned int new_d);
my_vector* padd(my_vector &c, unsigned int length, double specialchar=DBL_MAX);

//--------------------------------------------------- random_projection
random_projection::random_projection(const float _w,
          const unsigned int _k, const unsigned int _new_d,
          const size_t container_sz, const size_t _f_container_sz, const unsigned int _m)
          :w(_w),k(_k),m(_m),new_d(_new_d),f_container_sz(_f_container_sz){
  #if DEBUG
  cout<<"Constructing random_projection"<<'\n';
  #endif

  table_f_i=NULL;
}

random_projection::~random_projection(){
  #if DEBUG
  cout<<"Destructing random_projection"<<'\n';
  #endif
  if(table_f_i!=NULL){//if training was done
    for(unsigned int j=0;j<new_d;j++)
      delete table_f_i[j];
    delete[] table_f_i;
  }
}

//returns concatination of multiple f_i
unsigned long int random_projection::hash_function(my_vector &x){
  unsigned long int ans=0;
  for(unsigned int i=0;i<new_d;i++){
      ans=ans|table_f_i[i]->get_f_i(x);
      ans=ans<<1;
  }
  return ans;
}

//--------------------------------------------------- random_projection_vector
random_projection_vector::random_projection_vector(const float _w,
          const unsigned int _k, const unsigned int _new_d,
          const size_t container_sz, const size_t _f_container_sz, const unsigned int _m)
          :random_projection(_w,_k,_m,_new_d,_f_container_sz){
  #if DEBUG
  cout<<"Constructing random_projection_vector"<<'\n';
  #endif

  hash_table=new unordered_multimap<unsigned long int, my_vector*>;
  hash_table->reserve(container_sz);
  data=NULL;
}

random_projection_vector::~random_projection_vector(){
  #if DEBUG
  cout<<"Destructing random_projection_vector"<<'\n';
  #endif
  hash_table->clear();
  delete hash_table;
  if(data!=NULL){//if training was done
    data->clear();
    delete data;
  }
}

void random_projection_vector::train(list <my_vector> *train_data_set){
  #if DEBUG
  cout<<"Training cube_vector"<<'\n';
  #endif
  //coppy train_data_set list to data
  data=new list<my_vector>(*train_data_set);

  //create f_i table
  table_f_i=new f_i*[new_d];
  for(unsigned int j=0;j<new_d;j++)
    table_f_i[j]=new f_i(data->front().get_dimentions(),w,k,f_container_sz,m);

  //fill hash table
  for(auto it = data->begin(); it != data->end(); ++it)
      hash_table->insert({hash_function(*it),&*it});
}

pair<my_vector*, double> random_projection_vector::find_NN(my_vector &query,
                          double(*distance_metric)(my_vector&, my_vector&),//takes defaulf manhatan
                          unsigned int max_points, unsigned int prodes){
  my_vector *ans;
  double minn=DBL_MAX;
  unsigned long int* search_hash_numbers=get_search_buckets(hash_function(query),prodes,new_d);
  for(unsigned int j=0;j<prodes;j++){//serch all search_hash_numbers
    auto range = hash_table->equal_range(search_hash_numbers[j]);//returns all possible NNs
    for(auto it = range.first; it != range.second; ++it){
      double tmp=distance_metric(query, *it->second);
      if(minn>tmp){//if this is a better neighbor
        minn=tmp;
        ans=it->second;
      }
      max_points--;
      if(max_points==0){//if max_points reached
        delete[] search_hash_numbers;
        return make_pair(ans,minn);
      }
    }
  }
  delete[] search_hash_numbers;
  return make_pair(ans,minn);
}

list<my_vector*>* random_projection_vector::find_rNN(my_vector &query, double r,
                    double(*distance_metric)(my_vector&, my_vector&),
                    unsigned int max_points, unsigned int prodes){
  set<my_vector*> tmpset;
  unsigned long int* search_hash_numbers=get_search_buckets(hash_function(query),prodes,new_d);
  for(unsigned int j=0;j<prodes;j++){//serch all search_hash_numbers
    auto range = hash_table->equal_range(search_hash_numbers[j]);//returns all possible NNs
    for(auto it = range.first; it != range.second; ++it){
      double tmp=distance_metric(query, *it->second);
      if(tmp<=r)//if point has <=r distance
        tmpset.insert(it->second);
      max_points--;
      if(max_points==0){//if max_points reached
        delete[] search_hash_numbers;
        list<my_vector*> *ans=new list<my_vector*>;
        for(auto kk=tmpset.begin();kk!=tmpset.end();++kk)
          ans->push_back(*kk);
        return ans;
      }
    }
  }
  delete[] search_hash_numbers;
  list<my_vector*> *ans=new list<my_vector*>;
  for(auto kk=tmpset.begin();kk!=tmpset.end();++kk)
    ans->push_back(*kk);
  return ans;
}

//--------------------------------------------------- random_projection_curve
random_projection_curve::random_projection_curve(double _max_curve_sz, const float _w,
          const unsigned int _k, const unsigned int _new_d, double _pad_number,
          const size_t _container_sz, const size_t _f_container_sz,
          const unsigned int _m):random_projection(_w,_k,_new_d,_container_sz,_f_container_sz,_m),
          pad_number(_pad_number),max_curve_sz(_max_curve_sz){
  #if DEBUG
  cout<<"Constructing random_projection_curve"<<'\n';
  #endif
  hash_table=new unordered_multimap<long int, pair<my_curve*,my_vector*>>;
  hash_table->reserve(_container_sz);

  trained=false;
  gridhashfunctions=NULL;
  data=NULL;
}

random_projection_curve::~random_projection_curve(){
  #if DEBUG
  cout<<"Destructing random_projection_curve"<<'\n';
  #endif
  if(trained){
    for(auto it=hash_table->begin();it!=hash_table->end();++it)
      delete it->second.second;
    hash_table->clear();
    delete hash_table;
    if(data!=NULL){
      data->clear();
      delete data;
    }
    if(gridhashfunctions!=NULL)
      delete gridhashfunctions;
  }
  else{
    hash_table->clear();
    delete hash_table;
  }

}

void random_projection_curve::train(list<my_curve> *train_data_set){
  #if DEBUG
  cout<<"Training random_projection_curve"<<'\n';
  #endif
  if(train_data_set->size()!=0)
    trained=true;
  else
    return;

  data=new list<my_curve>(*train_data_set);

  gridhashfunctions = new GridHash(data->begin()->vectordimentions);

  //create f_i table
  table_f_i=new f_i*[new_d];
  for(unsigned int j=0;j<new_d;j++)
    table_f_i[j]=new f_i(max_curve_sz*data->front().vectordimentions,w,k,f_container_sz,m);

  for(auto it=data->begin();it!=data->end();++it){
      my_vector* final_vector=gridify_and_padd(*it);
      hash_table->insert({hash_function(*final_vector),make_pair(&*it, final_vector)});

  }
}

void random_projection_curve::train(list<pair<my_curve*, my_vector*>> *train_data_set){
  #if DEBUG
  cout<<"Training random_projection_curve"<<'\n';
  #endif
  if(train_data_set->size()!=0)
    trained=true;
  else
    return;
  data=NULL;
  gridhashfunctions=NULL;

  //create f_i table
  table_f_i=new f_i*[new_d];
  for(unsigned int j=0;j<new_d;j++)
    table_f_i[j]=new f_i(train_data_set->front().second->dim,w,k,f_container_sz,m);

  for(auto it=train_data_set->begin();it!=train_data_set->end();++it)
      hash_table->insert({hash_function(*it->second),*it});
}

pair<my_curve*, double> random_projection_curve::find_NN(pair<my_curve*,my_vector*> &query, unsigned int max_points, unsigned int prodes,
                  double (*distance_metric_curve)(my_curve&, my_curve&, double(*distance_metric_vector)(my_vector&, my_vector&)),
                  double(*distance_metric_vector)(my_vector&, my_vector&)){
  my_curve *ans;
  double minn=DBL_MAX;
  unsigned long int* search_hash_numbers=get_search_buckets(hash_function(*query.second),prodes,new_d);
  for(unsigned int j=0;j<prodes;j++){//serch all search_hash_numbers
    auto range = hash_table->equal_range(search_hash_numbers[j]);//returns all possible NNs
    for(auto it = range.first; it != range.second; ++it){
      double tmp=distance_metric_curve(*query.first, *it->second.first,distance_metric_vector);
      if(minn>tmp){//if this is a better neighbor
        minn=tmp;
        ans=it->second.first;
      }
      max_points--;
      if(max_points==0){//if max_points reached
        delete[] search_hash_numbers;
        return make_pair(ans,minn);
      }
    }
  }
  delete[] search_hash_numbers;
  return make_pair(ans,minn);

}

pair<my_curve*, double> random_projection_curve::find_NN(my_curve &query, unsigned int max_points, unsigned int prodes,
                  double (*distance_metric_curve)(my_curve&, my_curve&, double(*distance_metric_vector)(my_vector&, my_vector&)),
                  double(*distance_metric_vector)(my_vector&, my_vector&)){
  my_curve *ans;
  double minn=DBL_MAX;
  my_vector* final_vector=gridify_and_padd(query);
  unsigned long int* search_hash_numbers=get_search_buckets(hash_function(*final_vector),prodes,new_d);
  delete final_vector;
  for(unsigned int j=0;j<prodes;j++){//serch all search_hash_numbers
    auto range = hash_table->equal_range(search_hash_numbers[j]);//returns all possible NNs
    for(auto it = range.first; it != range.second; ++it){
      double tmp=distance_metric_curve(query, *it->second.first,distance_metric_vector);
      if(minn>tmp){//if this is a better neighbor
        minn=tmp;
        ans=it->second.first;
      }
      max_points--;
      if(max_points==0){//if max_points reached
        delete[] search_hash_numbers;
        return make_pair(ans,minn);
      }
    }
  }
  delete[] search_hash_numbers;
  return make_pair(ans,minn);
}

my_vector* random_projection_curve::gridify_and_padd(my_curve& curve, double pad_value){
  my_vector* tmp=gridhashfunctions->gridify(curve);
  my_vector* final_vector=padd(*tmp,max_curve_sz*curve.vectordimentions,pad_value);
  delete tmp;
  return final_vector;
}

//--------------------------------------------------- OTHER

//returns prev_mask changed by one bit
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

//returns a table sz=probes with all the dinary numbers with hamming
//distance(x)=0,1,... until the atble fills
unsigned long int* get_search_buckets(unsigned int x, unsigned int prodes, unsigned int new_d){
  unsigned long int *ans=new unsigned long int[prodes];
  int initial_mask = 0,mask;
  unsigned int index=1,j=0;

  ans[0]=x;
  while(prodes>index){
    //each iteration initial mask changes for hamming distance 0,1...
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
