#include <iostream>
#include <math.h>
#include <iterator>
#include <fstream>
#include <sstream>
#include <bits/stdc++.h>
#include <omp.h>
#include <limits>

#include "util.hpp"

#define DEBUG 0

using namespace std;

double Dtw( my_curve& x, my_curve& y, double(*distance_metric)(my_vector&, my_vector&)){
  unsigned int n=x.numofvectors,m=y.numofvectors;
  double OptValue [n+1][m+1];  // where the value of Dtw will be stored

  // initializing outer shells of OptValue with infinity
  OptValue[0][0]=0;
  for(unsigned int i=1;i<=n;i++)
    OptValue[i][0]=numeric_limits<float>::infinity();
  for(unsigned int i=1;i<=m;i++)
    OptValue[0][i]=numeric_limits<float>::infinity();
  //implementing the iterative algorithm to fill OptValue
  for(unsigned int i=1;i<=n;i++)
    for(unsigned int j=1;j<=m;j++)
      OptValue[i][j]=min(min(OptValue[i][j-1],OptValue[i-1][j]),OptValue[i-1][j-1])+distance_metric(x.get_vector(i-1), y.get_vector(j-1));
  //manhattan_distance is iterchangable with other norms
  return OptValue[n][m];
}

double manhattan_distance(my_vector& a, my_vector& b){
  double ans=0.0;
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

list <my_vector>*read_vector_file(string name){
  list <my_vector> *data=new list <my_vector>;
  ifstream infile(name);
  double num;
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

pair<my_vector*,double> brute_NN(list <my_vector> *data, my_vector &query, double(*distance_metric)(my_vector&, my_vector&)){
  my_vector *ans;
  double minn=DBL_MAX,tmp;

  for(list <my_vector> :: iterator it = data->begin(); it != data->end(); ++it){
    tmp=distance_metric(query, *it);
    if(minn>tmp){
      minn=tmp;
      ans=&*it;
    }

  }

  return make_pair(ans,minn);
}

pair<my_curve*,double> brute_NN_curve(list <my_curve> *data, my_curve &query,
                        double(*distance_metric_curve)(my_curve&, my_curve&, double(*distance_metric)(my_vector&, my_vector&)),
                        double(*distance_metric_vector)(my_vector&, my_vector&)){
  my_curve *ans;
  double minn=DBL_MAX,tmp;
  for(list <my_curve> :: iterator it = data->begin(); it != data->end(); ++it){
    tmp=distance_metric_curve(query, *it, distance_metric_vector);
    if(minn>tmp){
      minn=tmp;
      ans=&*it;
    }
  }
  return make_pair(ans,minn);
}

list <my_curve>* read_curve_file(string name, unsigned int max_curve_points){
  list <my_curve> *data=new list <my_curve>;
  ifstream infile(name);
  double num,num2;
  string str;
  char parenthesis; //used to remove parenthesis
  unsigned int i=0,input_N=0,vecnum=0;

  if (infile.good()){
    while(getline(infile, str)){
      // create temp curve
      istringstream ss(str);
      ss >> i;
      ss >> vecnum;
      my_curve curve(vecnum,2);
      curve.id=i;
      curve.numofvectors=vecnum;
      //filling the curve with info
      i=0;
      while(ss >> parenthesis){
        #if DEBUG
        cout << "trying to read vector with (id,vecnum)"<<curve.id<<","<<i << '\n';
        #endif
        ss>>num;
        ss>>parenthesis;
        ss>>num2;
        ss>>parenthesis;
        #if DEBUG
        std::cout << "read" <<num<<" "<<num2<<'\n';
        #endif
        curve.vectors[i]->coordinates[0]=num;
        curve.vectors[i++]->coordinates[1]=num2;
      }
      if(curve.numofvectors<=max_curve_points || max_curve_points==0){
        data->push_back(curve);
        input_N++;
      }
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

my_vector* padd(my_vector &c, unsigned int length, double specialchar){
  if(length<c.dim){
      cout<<"\n\n!!ERROR pad len not big enought!!\n\n";
      exit(1);
  }
  my_vector* padded_vector = new my_vector(length);
  unsigned int i = 0;
  for (i = 0; i < c.dim; i++) {
    padded_vector->coordinates[i] = c.coordinates[i];
  }
  for(;i<length;i++){
    padded_vector->coordinates[i] = specialchar;
  }
  return padded_vector;
}

my_curve* random_array(unsigned int k, unsigned int d){
  auto seed_t=chrono::system_clock::now().time_since_epoch();
  auto seed_m=chrono::duration_cast<chrono::nanoseconds>(seed_t);
  std::default_random_engine generator (seed_m.count());
  std::normal_distribution<double> distribution (0.0,1.0);
  my_curve* new_array = new my_curve(k,d);
  new_array->id=99999;// for debug
  //bulid new_array
  for (size_t i = 0; i < k; i++) {
    for (size_t j = 0; j < d; j++) {
      new_array->vectors[i]->coordinates[j] = distribution(generator);
    }
  }
  return new_array;
}

my_vector* multiply(my_curve& G, my_vector& U){//my curve is an array of its points
//check of matrix copatipility
  if(G.vectordimentions != U.dim){
    cout<<"\n\n!! ERROR multiply G*U fault !!\n\n";
    exit(1);
  }
  //initializing the result of the multiplication
  my_vector* result = new my_vector(G.numofvectors);
  result->id = 77777;   // this id is for debug
  // multiplication
  for (size_t i = 0; i < G.numofvectors; i++) {
    for (size_t j = 0; j < G.vectordimentions; j++) {
      result->coordinates[i] +=  U.coordinates[j]*G.vectors[i]->coordinates[j];
    }
  }
  return result;
}
