#include <iostream>
#include <math.h>
#include <unordered_set>

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

list<list<pair<unsigned int,unsigned int>*>*> *get_relevant_traversals(int m, int n,
                                      pair<unsigned int,unsigned int> **all_pairs);

pair<my_curve*,my_vector*> project_traversal_to_vector(my_curve* curve,
                  list<pair<unsigned int,unsigned int>*>* traversal, bool=false);

traversal_projection::traversal_projection(unsigned int _max_sz):max_sz(_max_sz){
  #if DEBUG
  cout<<"Constructing traversal_projection"<<'\n';
  #endif
  big_table=new list<list<pair<unsigned int,unsigned int>*>*>**[max_sz];
  for(unsigned int i=0;i<max_sz;i++)
    big_table[i]=new list<list<pair<unsigned int,unsigned int>*>*>*[max_sz];

  all_pairs=new pair<unsigned int,unsigned int>*[max_sz];
  for(unsigned int i=0;i<max_sz;i++)
      all_pairs[i]=new pair<unsigned int,unsigned int>[max_sz];

  for(unsigned int i=0;i<max_sz;i++)
    for(unsigned int j=i;j<max_sz;j++)
      all_pairs[i][j]=make_pair(i,j);

  for(unsigned int i=0;i<max_sz;i++)
    for(unsigned int j=i;j<max_sz;j++)
      big_table[i][j]=get_relevant_traversals(i+1,j+1,all_pairs);
}

traversal_projection::~traversal_projection(){
  #if DEBUG
  cout<<"Destructing traversal_projection"<<'\n';
  #endif

  for(unsigned int i=0;i<max_sz;i++){
    for(unsigned int j=i;j<max_sz;j++){
      for (auto it : *big_table[i][j]){
        it->clear();
        delete it;
      }
      big_table[i][j]->clear();
      delete big_table[i][j];
    }
    delete[] big_table[i];
  }
  delete[] big_table;

  for(unsigned int i=0;i<max_sz;i++)
    delete[] all_pairs[i];
  delete[] all_pairs;
}

void traversal_projection::print_big_table(){
  for(unsigned int i=0;i<max_sz;i++)
    for(unsigned int j=i;j<max_sz;j++){
      cout<<"\n\n-------------------------------\n"<<i<<" "<<j<<endl;
      for (auto it : *big_table[i][j]){
        for (auto jt : *it)
          cout<<jt->first<<","<<jt->second<<" ";
        cout<<endl;
      }
      cout<<endl;
    }

}

void traversal_projection::train(list <my_curve> *train_data_set){
  #if DEBUG
  cout<<"Training lsh_vector"<<'\n';
  #endif
  //coppy train_data_set list to data
  data=new list<my_curve>(*train_data_set);


}

//----------------------------------------------- OTHER

pair<my_curve*,my_vector*> project_traversal_to_vector(my_curve* curve,
                  list<pair<unsigned int,unsigned int>*>* traversal, bool is_query){
  my_vector *concat_vector=new my_vector(curve->numofvectors*curve->vectordimentions);
  concat_vector->id=curve->id;

  unsigned int indx=0;
  for(auto it=traversal->begin();it!=traversal->end();++it)
    for(unsigned int j=0;j<curve->vectordimentions;j++)
      if(!is_query)
        concat_vector[indx++]=curve->vectors[(*it)->first][j];//TODO *D
      else
        concat_vector[indx++]=curve->vectors[(*it)->second][j];//TODO *D

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
void make_relevant_traversals(int i, int j, int m, int n,
                      pair<unsigned int,unsigned int> **path, int pi,
                      my_unordered_set *relevant_squares,
                      list<list<pair<unsigned int,unsigned int>*>*> *relevant_traversals,
                      pair<unsigned int,unsigned int>** all_pairs){
    // Reached the bottom of the matrix so we are left with only option to move right
    if (i == m-1){
      for (int k = j; k < n; k++)
        path[pi + k - j] = &all_pairs[m-1][k];
      for (int l = 0; l < pi + n - j; l++)
        if(relevant_squares->find(*path[l])==relevant_squares->end())
          return;
      list<pair<unsigned int,unsigned int>*> *tmp=new list<pair<unsigned int,unsigned int>*>;
      for (int l = 0; l < pi + n - j; l++)
        tmp->push_back(path[l]);
      relevant_traversals->push_back(tmp);
      return;
    }

    // Reached the right corner of the matrix we are left with only the downward movement.
    if (j == n-1){
      for (int k = i; k < m; k++)
        path[pi + k - i] = &all_pairs[k][n-1];
      for (int l = 0; l < pi + m - i; l++)
        if(relevant_squares->find(*path[l])==relevant_squares->end())
          return;
      list<pair<unsigned int,unsigned int>*> *tmp=new list<pair<unsigned int,unsigned int>*>;
      for (int l = 0; l < pi + m - i; l++)
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
list<list<pair<unsigned int,unsigned int>*>*> *get_relevant_traversals(int m, int n,
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
