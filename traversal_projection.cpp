#include <iostream>
#include <math.h>
#include <unordered_set>
#include <bits/stdc++.h>

#include "traversal_projection.hpp"

#define DEBUG 0

using namespace std;

#ifdef BOOST_FUNCTIONAL_HASH_HASH_HPP
  #include <boost/functional/hash.hpp>
  typedef unordered_set<pair<unsigned int,unsigned int>,boost::hash<pair<unsigned int,unsigned int>>> my_unordered_set;
#else//!!!! risky messing with namespace std
  namespace std{
    template <> struct hash<std::pair<unsigned int, unsigned int>> {
        inline size_t operator()(const pair<int, int> &v) const {
            hash<unsigned int> uint_hasher;
            return uint_hasher(v.first) ^ uint_hasher(v.second);
        }
    };
  }
  typedef unordered_set<pair<unsigned int,unsigned int>> my_unordered_set;
#endif

template class traversal_projection<lsh_curve>;
template class traversal_projection<random_projection_curve>;

list<list<pair<unsigned int,unsigned int>*>*> *get_relevant_traversals(unsigned int m, unsigned int n,
                                      pair<unsigned int,unsigned int> **all_pairs);

pair<my_curve*,my_vector*> project_traversal_to_vector(my_curve* curve,
                  list<pair<unsigned int,unsigned int>*>* traversal, bool=false);

template<class T>
traversal_projection<T>::traversal_projection(unsigned int _max_sz):max_sz(_max_sz){
  #if DEBUG
  cout<<"Constructing traversal_projection"<<'\n';
  #endif

  all_pairs=new pair<unsigned int, unsigned int>*[max_sz];
  for(unsigned int i=0;i<max_sz;i++){
    all_pairs[i]=new pair<unsigned int, unsigned int>[max_sz];
    for(unsigned int j=0;j<max_sz;j++)
      all_pairs[i][j]=make_pair(i,j);
  }

  big_table=new list<list<pair<unsigned int, unsigned int>*>*>**[max_sz];
  for(unsigned int i=0;i<max_sz;i++){
    big_table[i]=new list<list<pair<unsigned int, unsigned int>*>*>*[max_sz];
    for(unsigned int j=0;j<max_sz;j++)
      big_table[i][j]=get_relevant_traversals(i+1,j+1,all_pairs);
  }
}

template<class T>
traversal_projection<T>::~traversal_projection(){
  #if DEBUG
  cout<<"Destructing traversal_projection"<<'\n';
  #endif

  for(unsigned int i=0;i<max_sz;i++){
    for(unsigned int j=0;j<max_sz;j++){
      for(auto it=lsh_table[i][j].begin();it!=lsh_table[i][j].end();++it)
        delete *it;
      lsh_table[i][j].clear();
    }
    delete[] lsh_table[i];
  }
  delete[] lsh_table;

  for(unsigned int i=0;i<max_sz;i++){
    for(unsigned int j=0;j<max_sz;j++){
      for(auto it=big_table[i][j]->begin();it!=big_table[i][j]->end();++it){
        (*it)->clear();
        delete *it;
      }
      big_table[i][j]->clear();
      delete big_table[i][j];
    }
    delete[] big_table[i];
  }
  delete[] big_table;

  data->clear();
  delete data;

  for(unsigned int i=0;i<max_sz;i++)
    delete[] all_pairs[i];
  delete[] all_pairs;
}

template<class T>
void traversal_projection<T>::print_big_table(){
  for(unsigned int i=0;i<max_sz;i++){
    for(unsigned int j=0;j<max_sz;j++){
      cout<<"All traversals for "<<i+1<<","<<j+1<<endl;
      for(auto it=big_table[i][j]->begin();it!=big_table[i][j]->end();++it){
        cout<<"traversal:\n";
        for(auto ij=(*it)->begin();ij!=(*it)->end();++ij){
          cout<<(*ij)->first<<","<<(*ij)->second<<" ";
        }
        cout<<endl;
      }
      cout<<endl;
    }
  }
}

template<>
void traversal_projection<lsh_curve>::train_lsh(list<my_curve> *train_data_set,
                        const unsigned int _l, const float _w,const unsigned int _k,
                        const size_t _container_sz, const unsigned int _m){
  #if DEBUG
  cout<<"Training traversal_projection<lsh_curve>"<<'\n';
  print_big_table();
  #endif

  data=new list<my_curve>(*train_data_set);

  lsh_table=new list<lsh_curve*>*[max_sz];
  for(unsigned int i=0;i<max_sz;i++){
    lsh_table[i]=new list<lsh_curve*>[max_sz];
    for(unsigned int j=0;j<max_sz;j++)
      for(auto it=big_table[i][j]->begin();it!=big_table[i][j]->end();++it)
        lsh_table[i][j].push_back(new lsh_curve(data->begin()->vectordimentions,2+i+j-1,_l,_w,_k,_container_sz,_m));
  }

  //search all data fo curves mikous i+1 and push them into same_curves
  for(unsigned int i=0;i<max_sz;i++){
    list<my_curve*> *same_curves=new list<my_curve*>;
    for(auto it=data->begin();it!=data->end();++it)
      if(it->numofvectors==i+1)
        same_curves->push_back(&*it);
    //train all lsh[i][*] with tmp
    for(unsigned int j=0;j<max_sz;j++){
      auto ii=lsh_table[i][j].begin();//for all lsh i j
      for(auto it=big_table[i][j]->begin();it!=big_table[i][j]->end();++it){//for all traversals i j
        list<pair<my_curve*,my_vector*>> *tmp=new list<pair<my_curve*,my_vector*>>;
        for(auto ij=same_curves->begin();ij!=same_curves->end();++ij){//for all curves mikous i
          tmp->push_back(project_traversal_to_vector(*ij,*it));
        }
        (*ii)->train(tmp);
        tmp->clear();
        delete tmp;
        ii++;
      }
    }
    same_curves->clear();
    delete same_curves;
  }

}

template<>
void traversal_projection<random_projection_curve>::train_cube(list <my_curve> *train_data_set,
                        unsigned int _max_curve_sz, const float _w,
                        const unsigned int _k, const unsigned int _new_d,
                        const size_t _container_sz, const size_t _f_container_sz, const unsigned int _m){
  #if DEBUG
  cout<<"Training traversal_projection<random_projection_curve>"<<'\n';
  print_big_table();
  #endif

  data=new list<my_curve>(*train_data_set);

  lsh_table=new list<random_projection_curve*>*[max_sz];
  for(unsigned int i=0;i<max_sz;i++){
    lsh_table[i]=new list<random_projection_curve*>[max_sz];
    for(unsigned int j=0;j<max_sz;j++)
      for(auto it=big_table[i][j]->begin();it!=big_table[i][j]->end();++it)
        lsh_table[i][j].push_back(new random_projection_curve(2+i+j-1,_w,_k,_new_d,_container_sz,_f_container_sz,_m));
  }

  //search all data fo curves mikous i+1 and push them into same_curves
  for(unsigned int i=0;i<max_sz;i++){
    list<my_curve*> *same_curves=new list<my_curve*>;
    for(auto it=data->begin();it!=data->end();++it)
      if(it->numofvectors==i+1)
        same_curves->push_back(&*it);
    //train all lsh[i][*] with tmp
    for(unsigned int j=0;j<max_sz;j++){
      auto ii=lsh_table[i][j].begin();//for all lsh i j
      for(auto it=big_table[i][j]->begin();it!=big_table[i][j]->end();++it){//for all traversals i j
        list<pair<my_curve*,my_vector*>> *tmp=new list<pair<my_curve*,my_vector*>>;
        for(auto ij=same_curves->begin();ij!=same_curves->end();++ij){//for all curves mikous i
          tmp->push_back(project_traversal_to_vector(*ij,*it));
        }
        (*ii)->train(tmp);
        tmp->clear();
        delete tmp;
        ii++;
      }
    }
    same_curves->clear();
    delete same_curves;
  }

}

template<class T>
std::pair<my_curve*, double> traversal_projection<T>::find_NN(my_curve &query,
                double (*distance_metric_curve)(my_curve&, my_curve&, double(*distance_metric_vector)(my_vector&, my_vector&)),
                double(*distance_metric_vector)(my_vector&, my_vector&)){
  #if DEBUG
  cout<<"entering traversal_projection::find_NN\n";
  #endif

  int i=query.numofvectors-1,j=i;

  my_curve* ans;
  double minn=DBL_MAX;

  // for(unsigned int k=-1;k<=1;k++){
  //   i+=k;
  //   if(i==-1 || i==max_sz)
  //     continue;
    auto ij=lsh_table[i][j].begin();
    for(auto it=big_table[i][j]->begin();it!=big_table[i][j]->end();++it){
      pair<my_curve*,my_vector*> query2=project_traversal_to_vector(&query,*it,true);
      pair<my_curve*,double> tmp=(*ij)->find_NN(query2);
      if(minn>tmp.second){
        ans=tmp.first;
        minn=tmp.second;
      }
      ij++;
      delete query2.second;
    }
  // }



  return make_pair(ans,minn);
}

//----------------------------------------------- OTHER

pair<my_curve*,my_vector*> project_traversal_to_vector(my_curve* curve,
                  list<pair<unsigned int,unsigned int>*>* traversal, bool is_query){
  #if DEBUG
  cout<<"entering project_traversal_to_vector for"<<curve->vectordimentions<<"d curve with "
  <<curve->numofvectors<<" points and traversal: \n";
  for(auto it=traversal->begin();it!=traversal->end();++it)
    cout<<(*it)->first<<","<<(*it)->second<<" ";
  cout<<endl;
  #endif
  my_vector *concat_vector=new my_vector(traversal->size()*curve->vectordimentions);
  concat_vector->id=curve->id;

  unsigned int indx=0;
  for(auto it=traversal->begin();it!=traversal->end();++it)
    for(unsigned int j=0;j<curve->vectordimentions;j++)
      if(!is_query)
        concat_vector->coordinates[indx++]=curve->vectors[(*it)->first]->coordinates[j];//TODO *D
      else
        concat_vector->coordinates[indx++]=curve->vectors[(*it)->second]->coordinates[j];//TODO *D

  return make_pair(curve,concat_vector);
}

//reteurns a unordered_set containing the squares of the diagonal
my_unordered_set *get_diagonal(unsigned int x_sz, unsigned int y_sz){
  double b=y_sz*1.0/x_sz;
  unsigned int tmp_x,tmp_y;

  my_unordered_set *coo=new my_unordered_set;

  for(unsigned int y=0;y<=y_sz;y++){
    tmp_x=floor(y/b);
    if(tmp_x==x_sz && y==y_sz) break;
    coo->insert(make_pair(tmp_x, y));
  }
  for(unsigned int x=0;x<=x_sz;x++){
    tmp_y=floor(b*x);
    if(x==x_sz && tmp_y==y_sz) break;
    coo->insert(make_pair(x,tmp_y));
  }

  #if DEBUG
  cout<<x_sz<<"*"<<y_sz<<" table"<<endl;
  for(pair<unsigned int, unsigned int> i : *coo)
    cout<<"x= "<<i.first<<"\ty= "<<i.second<<endl;
  #endif

  return coo;
}

//reteurns a unordered_set containing the squares of the diagonal and y+-1
my_unordered_set *get_diagonal_plus(my_unordered_set *traversals_diagonal,unsigned int y_sz){
  my_unordered_set plus1;
  my_unordered_set minus1;

  for(auto it=traversals_diagonal->begin();it!=traversals_diagonal->end();++it){
    if(it->second+1<y_sz)
      plus1.insert(make_pair(it->first,it->second+1));
    if((int)it->second-1>=0)
    minus1.insert(make_pair(it->first,it->second-1));
  }

  traversals_diagonal->insert(plus1.begin(), plus1.end());
  traversals_diagonal->insert(minus1.begin(), minus1.end());

  #if DEBUG
  for(pair<unsigned int, unsigned int> i : *traversals_diagonal)
    cout<<"x= "<<i.first<<"\ty= "<<i.second<<endl;
  #endif

  return traversals_diagonal;
}

//modifies relevant_traversals to a list containing all the relevant traversals(list)
void make_relevant_traversals(unsigned int i, unsigned  int j, unsigned int m, unsigned int n,
                      pair<unsigned int,unsigned int> **path, int pi,
                      my_unordered_set *relevant_squares,
                      list<list<pair<unsigned int,unsigned int>*>*> *relevant_traversals,
                      pair<unsigned int,unsigned int>** all_pairs){
    if(relevant_squares->find(make_pair(i,j))==relevant_squares->end())
      return;
    // Reached the bottom of the matrix so we are left with only option to move right
    if (i == m-1){
      for (unsigned int k = j; k < n; k++)
        path[pi + k - j] = &all_pairs[m-1][k];
      list<pair<unsigned int,unsigned int>*> *tmp=new list<pair<unsigned int,unsigned int>*>;
      for (unsigned int l = 0; l < pi + n - j; l++)
        tmp->push_back(path[l]);
      relevant_traversals->push_back(tmp);
      return;
    }

    // Reached the right corner of the matrix we are left with only the downward movement.
    if (j == n-1){
      for (unsigned int k = i; k < m; k++)
        path[pi + k - i] = &all_pairs[k][n-1];
      list<pair<unsigned int,unsigned int>*> *tmp=new list<pair<unsigned int,unsigned int>*>;
      for (unsigned int l = 0; l < pi + m - i; l++)
        tmp->push_back(path[l]);
      relevant_traversals->push_back(tmp);
      return;
    }

    // Add the current cell to the path being generated
    path[pi] = &all_pairs[i][j];

    // Print all the paths that are possible after moving down
    make_relevant_traversals(i+1, j, m, n, path, pi + 1,relevant_squares,relevant_traversals,all_pairs);

    // Print all the paths that are possible after moving right
    make_relevant_traversals(i, j+1, m, n, path, pi + 1,relevant_squares,relevant_traversals,all_pairs);

}

//calls get_diagonal, get_diagonal_plus and make_relevant_traversals and returns a list of lists
//filled with all the relevant traversals
list<list<pair<unsigned int,unsigned int>*>*> *get_relevant_traversals(unsigned int m, unsigned int n,
                                    pair<unsigned int,unsigned int>** all_pairs){
  my_unordered_set *relevant_squares;
  relevant_squares=get_diagonal(m,n);
  #if DEBUG
  cout<<"get_diagonal done!!\n";
    cout<<"---------------\n";
  #endif
  relevant_squares=get_diagonal_plus(relevant_squares,n);
  #if DEBUG
  cout<<"get_diagonal_plus done!!\n";
  #endif

  list<list<pair<unsigned int,unsigned int>*>*> *relevant_traversals=new list<list<pair<unsigned int,unsigned int>*>*>;
  pair<unsigned int,unsigned int> **path = new pair<unsigned int,unsigned int>*[m+n];

  make_relevant_traversals(0, 0, m, n, path, 0,relevant_squares,relevant_traversals,all_pairs);
  #if DEBUG
  cout<<"make_relevant_traversals done!!\n";
  #endif

  delete relevant_squares;
  delete[] path;

  return relevant_traversals;
}
