#include <iostream>
#include <bits/stdc++.h>
#include <iterator>
#include <algorithm>

#include "lsh.hpp"
#include "g_i.hpp"
#include "util.hpp"
#include "GridHash.hpp"

#define DEBUG 0

using namespace std;

my_vector* padd(my_vector &c, unsigned int length, double specialchar=DBL_MAX);

//----------------------------------------------lsh
lsh::lsh(unsigned int dimentions, const unsigned int _l, const float _w,
          const unsigned int _k, const unsigned int _m):w(_w),k(_k),m(_m),l(_l){
  #if DEBUG
  cout<<"Constructing lsh"<<'\n';
  #endif
  table_g_i=new g_i*[l];
  for(unsigned int i=0;i<l;i++)
    table_g_i[i]=new g_i(dimentions,w,k,m);
}

lsh::~lsh(){
  #if DEBUG
  cout<<"Destructing lsh"<<'\n';
  #endif
  for(unsigned int i=0;i<l;i++)
    delete table_g_i[i];
  delete[] table_g_i;
}

//----------------------------------------------lsh_vector
lsh_vector::lsh_vector(unsigned int dimentions, const unsigned int _l, const float _w,
          const unsigned int _k, const size_t _container_sz,
          const unsigned int _m):lsh(dimentions,_l,_w,_k,_m){
  #if DEBUG
  cout<<"Constructing lsh_vector"<<'\n';
  #endif
  hash_table=new unordered_multimap<long int, my_vector*>*[l];
  for(unsigned int i=0;i<l;i++){
    hash_table[i]=new unordered_multimap<long int, my_vector*>;
    hash_table[i]->reserve(_container_sz);
  }

  data=NULL;
}

lsh_vector::~lsh_vector(){
  #if DEBUG
  cout<<"Destructing lsh_vector"<<'\n';
  #endif
  for(unsigned int i=0;i<l;i++){
    hash_table[i]->clear();
    delete hash_table[i];
  }
  delete[] hash_table;
  if(data!=NULL){//if training was done
    data->clear();
    delete data;
  }
}

void lsh_vector::train(list <my_vector> *train_data_set){
  #if DEBUG
  cout<<"Training lsh_vector"<<'\n';
  #endif
  //coppy train_data_set list to data
  data=new list<my_vector>(*train_data_set);

  //fill hash table
  for(unsigned int i=0;i<l;i++)
    for(auto it = data->begin(); it != data->end(); ++it)
      hash_table[i]->insert({table_g_i[i]->get_g_x(*it),&*it});
}

pair<my_vector*, double> lsh_vector::find_NN(my_vector &query,
                  double (*distance_metric)(my_vector&, my_vector&)){
  my_vector *ans;
  double minn=DBL_MAX;
  for(unsigned int i=0;i<l;i++){
    auto range = hash_table[i]->equal_range(table_g_i[i]->get_g_x(query));//returns all possible NNs
    for(auto it = range.first; it != range.second; ++it){
      double tmp=distance_metric(query,*it->second);
      if(minn>tmp){//if this is a better neighbor
        minn=tmp;
        ans=it->second;
      }
      //TODO 3L early abandonment
    }
  }
  return make_pair(ans,minn);
}

list<my_vector*>* lsh_vector::find_rNN(my_vector &query, double r, double (*distance_metric)(my_vector&, my_vector&)){
  set<my_vector*> tmpset;
  for(unsigned int i=0;i<l;i++){
    auto range = hash_table[i]->equal_range(table_g_i[i]->get_g_x(query));//returns all possible NNs
    for(auto it = range.first; it != range.second; ++it){
      double tmp=distance_metric(query, *it->second);
      if(tmp<=r)//if point has <=r distance
        tmpset.insert(it->second);
    }
  }
  list<my_vector*> *ans=new list<my_vector*>;
  for(auto kk=tmpset.begin();kk!=tmpset.end();++kk)
    ans->push_back(*kk);
  return ans;
}

//----------------------------------------------lsh_curve
lsh_curve::lsh_curve(unsigned int vector_dimentions, unsigned int _max_curve_sz, const unsigned int _l, const float _w,
          const unsigned int _k, const size_t _container_sz,
          const unsigned int _m):lsh(vector_dimentions*_max_curve_sz,_l,_w,_k,_m),
          max_curve_sz(_max_curve_sz){
  #if DEBUG
  cout<<"Constructing lsh_curve"<<'\n';
  #endif
  hash_table=new unordered_multimap<long int, pair<my_curve*,my_vector*>>*[l];
  for(unsigned int i=0;i<l;i++){
    hash_table[i]=new unordered_multimap<long int, pair<my_curve*,my_vector*>>;
    hash_table[i]->reserve(_container_sz);
  }

  matching=NULL;
  gridhashfunctions=NULL;
  data=NULL;
}

lsh_curve::~lsh_curve(){
  #if DEBUG
  cout<<"Destructing lsh_curve"<<'\n';
  #endif
  for(unsigned int i=0;i<l;i++){
    hash_table[i]->clear();
    delete hash_table[i];
  }
  delete[] hash_table;
  if(gridhashfunctions!=NULL){
    for(unsigned int i=0;i<l;i++)
      delete gridhashfunctions[i];
    delete[] gridhashfunctions;
  }
  if(matching!=NULL){
    if(data==NULL)//if trained bu pairs then .second pointers show to the same item
      for(auto it=matching[0]->begin();it!=matching[0]->end();++it)
        delete it->second;
    else
      for(unsigned int i=0;i<l;i++)
        for(auto it=matching[i]->begin();it!=matching[i]->end();++it)
          delete it->second;
    for(unsigned int i=0;i<l;i++){
      matching[i]->clear();
      delete matching[i];
    }
    delete[] matching;
  }
  if(data!=NULL){
    data->clear();
    delete data;
  }
}

void lsh_curve::train(list<my_curve> *train_data_set){
  #if DEBUG
  cout<<"Training lsh_curve"<<'\n';
  #endif
  gridhashfunctions = new GridHash*[l];
  matching=new list<pair<my_curve*,my_vector*>>*[l];
  for(unsigned int i=0;i<l;i++){
    gridhashfunctions[i]=new GridHash(train_data_set->begin()->vectordimentions);
    matching[i]=new list<pair<my_curve*,my_vector*>>;
  }

  data=new list<my_curve>(*train_data_set);

  for(auto it=data->begin();it!=data->end();++it){
    for(unsigned int i=0;i<l;i++){
      my_vector* final_vector=gridify_and_padd(*it,i);
      matching[i]->push_back(make_pair(&*it, final_vector));
      hash_table[i]->insert({table_g_i[i]->get_g_x(*final_vector),make_pair(&*it, final_vector)});
    }
  }
}

void lsh_curve::train(list<pair<my_curve*, my_vector*>> *train_data_set){
  #if DEBUG
  cout<<"Training lsh_curve"<<'\n';
  #endif
  data=NULL;

  matching=new list<pair<my_curve*,my_vector*>>*;
  matching[0]=new list<pair<my_curve*,my_vector*>>(*train_data_set);

  for(auto it=train_data_set->begin();it!=train_data_set->end();++it)
    for(unsigned int i=0;i<l;i++)
      hash_table[i]->insert({table_g_i[i]->get_g_x(*it->second),*it});
}

pair<my_curve*, double> lsh_curve::find_NN(pair<my_curve*,my_vector*> &query,
                  double (*distance_metric_curve)(my_curve&, my_curve&, double(*distance_metric_vector)(my_vector&, my_vector&)),
                  double(*distance_metric_vector)(my_vector&, my_vector&)){
  #if DEBUG
  cout<<"entering lsh_curve::find_NN\n";
  #endif
  my_curve *ans;
  double minn=DBL_MAX;
  for(unsigned int i=0;i<l;i++){
    auto range = hash_table[i]->equal_range(table_g_i[i]->get_g_x(*query.second));//returns all possible NNs
    for(auto it = range.first; it != range.second; ++it){
      double tmp=distance_metric_curve(*query.first,*it->second.first,distance_metric_vector);
      if(minn>tmp){//if this is a better neighbor
        minn=tmp;
        ans=it->second.first;
      }
      //TODO 3L early abandonment
    }
  }
  return make_pair(ans,minn);
}

pair<my_curve*, double> lsh_curve::find_NN(my_curve &query,
                  double (*distance_metric_curve)(my_curve&, my_curve&, double(*distance_metric_vector)(my_vector&, my_vector&)),
                  double(*distance_metric_vector)(my_vector&, my_vector&)){
  my_curve *ans;
  double minn=DBL_MAX;
  for(unsigned int i=0;i<l;i++){
    my_vector *vector_query=gridify_and_padd(query,i);
    auto range = hash_table[i]->equal_range(table_g_i[i]->get_g_x(*vector_query));//returns all possible NNs
    delete vector_query;
    for(auto it = range.first; it != range.second; ++it){
      double tmp=distance_metric_curve(query,*it->second.first,distance_metric_vector);
      if(minn>tmp){//if this is a better neighbor
        minn=tmp;
        ans=it->second.first;
      }
      //TODO 3L early abandonment
    }
  }
  return make_pair(ans,minn);
}

list<my_curve*>* lsh_curve::find_rNN(my_curve &query, double r,
                        double (*distance_metric_curve)(my_curve&, my_curve&, double(*distance_metric_vector)(my_vector&, my_vector&)),
                        double(*distance_metric_vector)(my_vector&, my_vector&)){
  set<my_curve*> tmpset;
  for(unsigned int i=0;i<l;i++){
    my_vector *vector_query=gridify_and_padd(query,i);
    auto range = hash_table[i]->equal_range(table_g_i[i]->get_g_x(*vector_query));//returns all possible NNs
    for(auto it = range.first; it != range.second; ++it){
      double tmp=distance_metric_curve(query, *it->second.first,distance_metric_vector);
      if(tmp<=r)//if point has <=r distance
        tmpset.insert(it->second.first);
    }
  }
  list<my_curve*> *ans=new list<my_curve*>;
  for(auto kk=tmpset.begin();kk!=tmpset.end();++kk)
    ans->push_back(*kk);
  return ans;
}

my_vector* lsh_curve::gridify_and_padd(my_curve& curve, unsigned int iteration){//prosoxi iparxi idia sto random_projection.cpp
  my_vector* tmp=gridhashfunctions[iteration]->gridify(curve);
  my_vector* final_vector=padd(*tmp,max_curve_sz*curve.vectordimentions);//TODO add specialchar
  delete tmp;
  return final_vector;
}
