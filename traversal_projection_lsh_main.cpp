#include <iostream>
#include <fstream>
#include <chrono>
#include <unistd.h>
#include <string.h>
#include <bits/stdc++.h>

#include "my_curve.hpp"
#include "util.hpp"
#include "traversal_projection.hpp"

#define DEBUG 0

#define MAX_CURVE_POINTS 6
//TODO MAX_CURVE_POINTS from command lsh_container_size

using namespace std;

int main(int argc, char *argv[]){
  //w is the window in h
  int k=4,l=5,m=999999;
  size_t lsh_container_size=100;//not needed by project
  float w=0.01;

  char input_file_data[100]("./.atomignore/trajectories_dataset_cut");
  char input_file_queries[100]("./.atomignore/queriecurve");
  char out_file[100]("curve_projection_lsh_out");

  //------------------------------------parse arguments
  int opt;
  while((opt = getopt(argc, argv, "d:q:k:L:o:w:m:c:"))!=-1){
    switch(opt){
      case 'd':
        cout<<optarg<<endl;
        strcpy(input_file_data,optarg);
        break;
      case 'q':
        strcpy(input_file_queries,optarg);
        break;
      case 'k':
        k=atoi(optarg);
        break;
      case 'L':
        l=atoi(optarg);
        break;
      case 'o':
        strcpy(out_file,optarg);
        break;
      case 'w':
        w=atoi(optarg);
        break;
      case 'm':
        m=atoi(optarg);
        break;
      case 'c':
        lsh_container_size=atoi(optarg);
        break;
      default:
        cout<<"!! WRONG ARGUMENTS !!\n";
        exit(1);
    }
  }
  cout<<"program running with:\n\tdata_file= "<<input_file_data<<
    "\n\tquery_file= "<<input_file_queries<<"\n\tout_file= "<<out_file<<
    "\n\tk= "<<k<<"\n\tl= "<<l<<"\n\tw= "<<w<<"\n\tm= "<<m
    <<"\n\tf_container_sz= "<<lsh_container_size<<endl<<endl;

  //------------------------------------create out file
  ofstream ofile;
  ofile.open(out_file);
  if(!ofile.is_open()){
    cout<<"!! ERROR OUTFILE !!\n";
    exit(1);
  }

  // cout.setstate(ios::failbit);
  //------------------------------------read input files
  list <my_curve> *data=read_curve_file(input_file_data,MAX_CURVE_POINTS);
  list <my_curve> *queries=read_curve_file(input_file_queries,MAX_CURVE_POINTS);
  cout<<"read files done\n";

  //------------------------------------create and train model
  traversal_projection<lsh_curve> lsh_model(MAX_CURVE_POINTS);
  lsh_model.train_lsh(data,l,w,k,lsh_container_size,99999.99999,m);
  cout<<"traversal_projection<lsh_curve> training done!!\n";

  // cout.clear();
  //------------------------------------fill out file, running bruteNN and cubeNN
  double AF_max=0.0,AF_avg=0.0,AF;
  long int average_time=0,average_time_true=0;
  unsigned int total=0;
  using namespace std::chrono;
  for(list <my_curve> :: iterator it = queries->begin(); it != queries->end(); ++it){
    auto start = high_resolution_clock::now();
    pair<my_curve*,double> nn_brute=brute_NN_curve(data,*it,Dtw,manhattan_distance);
    auto stop = high_resolution_clock::now();
    auto duration_brute = duration_cast<nanoseconds>(stop - start);

    start = high_resolution_clock::now();
    pair<my_curve*,double> nn_lsh=lsh_model.find_NN(*it,2,Dtw,manhattan_distance);
    stop = high_resolution_clock::now();
    auto duration_lsh = duration_cast<nanoseconds>(stop - start);

    if(nn_brute.second==0.0){
      cout<<"Warining: true distance is ~=0! querry "<<it->id<<" does not count\n";
      continue;
    }
    if(nn_lsh.second==DBL_MAX){
      cout<<"Warining: calculated distance is ~=inf! querry "<<it->id<<" does not count\n";
      continue;
    }
    AF=nn_lsh.second/nn_brute.second;
    AF_max=max(AF,AF_max);
    AF_avg+=AF;
    average_time+=duration_lsh.count();
    average_time_true+=duration_brute.count();

    ofile<<"Query: "<<it->id<<endl;
    ofile<<"Nearest neighbor: "<<nn_lsh.first->id<<endl;
    ofile<<"distanceLSH: "<<nn_lsh.second<<endl;
    ofile<<"distanceTrue: "<<nn_brute.second<<endl;
    ofile<<"tLSH: "<<duration_lsh.count()<<endl;
    ofile<<"tTrue: "<<duration_brute.count()<<endl;

    total++;
  }

  if(total!=0){
    AF_avg/=total;
    average_time/=total;
    average_time_true/=total;
    cout<<"AF_max= "<<AF_max<<"\tAF_avg= "<<AF_avg<<"\naverage_time=\t\t"<<average_time<<" nanoseconds\navarage_time_true=\t"<<average_time_true<<" nanoseconds\n";
  }
  else
    cout<<"Found no ANN! plz change parameters\n";


  //------------------------------------clearing memmory
  ofile.close();
  data->clear();
  queries->clear();
  delete data;
  delete queries;

  return 0;
}
