#include <iostream>
#include <math.h>
#include <iterator>
#include <fstream>
#include <sstream>
#include <bits/stdc++.h>
#include <omp.h>

#include "util.hpp"

#define DEBUG 0

using namespace std;


float Dtw( my_curve& x, my_curve& y)
{
  unsigned int n=x.numofvectors,m=y.numofvectors;
  float OptValue [n+1][m+1];  // where the value of Dtw will be stored

  // initializing outer shells of OptValue with infinity
  OptValue[0][0]=0;
  for(unsigned int i=1;i<=n;i++)
    OptValue[i][0]=numeric_limits<float>::infinity();
  for(unsigned int i=1;i<=m;i++)
    OptValue[0][i]=numeric_limits<float>::infinity();
  //implementing the iterative algorithm to fill OptValue
  for(unsigned int i=1;i<=n;i++)
    for(unsigned int j=1;j<=m;j++)
      OptValue[i][j]=min(min(OptValue[i][j-1],OptValue[i-1][j]),OptValue[i-1][j-1])+manhattan_distance(x.get_vector(i), y.get_vector(j));
  //manhattan_distance is iterchangable with other norms
  //TODO return 1 opt solution
  return OptValue[n][m];
}

int manhattan_distance(my_vector a, my_vector b){
  int ans=0;
  if(a.get_dimentions()!=b.get_dimentions()){
    cerr<<"\n\n!!manhattan_distance dimentions ERROR!!\n\n";
    exit(1);
  }
  for(unsigned int i=0;i<a.get_dimentions();i++)
    ans+=abs(a.coordinates[i]-b.coordinates[i]);

  // printf("%d (%d,%d) (%d,%d) = %d\n",a.get_dimentions(),a.coordinates[0],a.coordinates[1]
  // ,b.coordinates[0],b.coordinates[1],ans );

  return ans;
}

list <my_vector>* read_vector_file(string name){
  list <my_vector> *data=new list <my_vector>;
  ifstream infile(name);
  int num;
  string str;
  unsigned int i=0,input_N=0;

  if (infile.good()){
    while(getline(infile, str)){
      istringstream ss(str);
      my_vector vec(count(str.begin(),str.end(),' ')-1);
      ss >> i;
      vec.id=i;
      i=0;
      while(ss >> num)
        vec.coordinates[i++]=num;
      #if DEBUG
      vec.print_vec();
      #endif
      data->push_back(vec);
      input_N++;
    }
  }
  else{
    cerr << "\n\n!! INPUT FILE ERROR !!\n\n";
    exit(1);
  }

  #if DEBUG
  cout<<"Total input_N= "<<input_N<<endl;
  #endif

  infile.close();
  return data;
}

template <typename T>
T modpow(T base, T exp, T modulus){
  base %= modulus;
  T result = 1;
  while (exp > 0) {
    if (exp & 1) result = (result * base) % modulus;
    base = (base * base) % modulus;
    exp >>= 1;
  }
  return result;
}
template int modpow<int>(int, int, int);
template unsigned int modpow<unsigned int>(unsigned int, unsigned int, unsigned int);

short int hammingDistance(short int n1, short int n2){
    int x = n1 ^ n2;
    int setBits = 0;

    while (x > 0) {
        setBits += x & 1;
        x >>= 1;
    }

    return setBits;
}

int* get_hamming_distance_01(int x, unsigned int ans_size){
  int *ans=new int[ans_size];
  int mask = 1;

  ans[0]=x;
  for(unsigned int i=1;i<ans_size;i++){
    ans[i]=x^mask;
    mask=mask<<1;
  }

  return ans;
}

pair<my_vector*,int> brute_NN(list <my_vector> *data, my_vector &query){
  my_vector *ans;
  int minn=INT_MAX,tmp;

  for(list <my_vector> :: iterator it = data->begin(); it != data->end(); ++it){
    tmp=manhattan_distance(query, *it);
    if(minn>tmp){
      minn=tmp;
      ans=&*it;
    }

  }

  return make_pair(ans,minn);
}

float brute_NC(list <my_curve> *data, my_curve query){
  float ans=INT_MAX,tmp;

  for(list <my_curve> :: iterator it = data->begin(); it != data->end(); ++it){
    tmp=Dtw(query, *it);
    ans=min(ans,tmp);
  }
  return ans;
}
