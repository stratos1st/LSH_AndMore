#include <iostream>
#include <fstream>
#include <chrono>
#include <unistd.h>
#include <string.h>
#include <bits/stdc++.h>

#include "my_curve.hpp"
#include "util.hpp"
#include "lsh.hpp"

#define DEBUG 0

#define MAX_CURVE_POINTS 10
//TODO MAX_CURVE_POINTS from command lsh_container_size
//TODO container sz experiments
//TODO chagne DBL_MAX to inf maybe

using namespace std;

int main(int argc, char *argv[]){
  //k is the k from g(x)
  //new_d is the d' from random projection aka k in arguments
  //w is the window in h
  int k=4,max_points=INT_MAX,prodes=25,m=99999999,l=5;
  float w=0.001;
  double r=0.0001;//used for bonus not required by project
  size_t lsh_container_size=100;//not needed by project

  char input_file_data[100]("./.atomignore/trajectories_dataset_cut");
  char input_file_queries[100]("./.atomignore/queriecurve");
  char out_file[100]("curve_grid_lsh_out");

  //------------------------------------parse arguments
  int opt;
  while((opt = getopt(argc, argv, "d:q:L:o:w:M:p:r:m:c"))!=-1){
    switch(opt){
      case 'd':
        cout<<optarg<<endl;
        strcpy(input_file_data,optarg);
        break;
      case 'q':
        strcpy(input_file_queries,optarg);
        break;
      case 'o':
        strcpy(out_file,optarg);
        break;
      case 'w':
        w=atoi(optarg);
        break;
      case 'M':
        max_points=atoi(optarg);
        break;
      case 'p':
        prodes=atoi(optarg);
        break;
      case 'r':
        r=atoi(optarg);
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

  //------------------------------------info passed
  cout<<"program running with:\n\tdata_file= "<<input_file_data<<
    "\n\tquery_file= "<<input_file_queries<<"\n\tout_file= "<<out_file
    <<"\n\tm= "<<m<<"\n\tM= "<<max_points<<"\n\tprodes= "<<prodes
    <<"\n\tw= "<<w<<"\n\tk= "<<k<<"\n\tr= "<<r<<"\n\tlsh_container_size= "<<
    lsh_container_size<<endl<<endl;

  //------------------------------------create out file
  ofstream ofile;
  ofile.open(out_file);
  if(!ofile.is_open()){
    cout<<"!! ERROR OUTFILE !!\n";
    exit(1);
  }

  //------------------------------------read input files
  list <my_curve> *data=read_curve_file(input_file_data,MAX_CURVE_POINTS);
  list <my_curve> *queries=read_curve_file(input_file_queries,MAX_CURVE_POINTS);
  cout<<"reading files done!!\n";

  //------------------------------------create and train model
  lsh_curve lsh_model(data->front().vectors[0]->get_dimentions(),MAX_CURVE_POINTS,l,w,k,lsh_container_size,m);
  lsh_model.train(data);
  cout<<"lsh_curve training done!!\n";

  //------------------------------------fill out file, running bruteNN and cubeNN
  double AF_max=0.0,AF_avg=0.0,AF;
  long int average_time=0,average_time_true=0;
  unsigned int total=0;
  using namespace std::chrono;
  for(list <my_curve> :: iterator it = queries->begin(); it != queries->end(); ++it){
    auto start = high_resolution_clock::now();
    pair<my_curve*,double> nn_brute=brute_NN_curve(data,*it,Dtw);
    auto stop = high_resolution_clock::now();
    auto duration_brute = duration_cast<nanoseconds>(stop - start);

    start = high_resolution_clock::now();
    pair<my_curve*,double> nn_cube=lsh_model.find_NN(*it,Dtw);
    stop = high_resolution_clock::now();
    auto duration_lsh = duration_cast<nanoseconds>(stop - start);

    if(nn_brute.second==0.0){
      cout<<"Warining: real distance is ~=0! querry "<<it->id<<" does not count\n";
      continue;
    }
    if(nn_cube.second==DBL_MAX){
      cout<<"Warining: predicted distance is ~=inf! querry "<<it->id<<" does not count\n";
      continue;
    }

    AF=nn_cube.second/nn_brute.second;
    AF_max=max(AF,AF_max);
    AF_avg+=AF;
    average_time+=duration_lsh.count();
    average_time_true+=duration_brute.count();

    ofile<<"Query: "<<it->id<<endl;
    ofile<<"Nearest neighbor: "<<nn_cube.first->id<<endl;
    ofile<<"distanceLsh: "<<nn_cube.second<<endl;
    ofile<<"distanceTrue: "<<nn_brute.second<<endl;
    ofile<<"tLsh: "<<duration_lsh.count()<<endl;
    ofile<<"tTrue: "<<duration_brute.count()<<endl;
    // ofile<<"R-near neighbors: "<<endl;

    // list<my_curve*>* rNNs=lsh_model.find_rNN(*it,r,Dtw);
    // for(list <my_curve*>::iterator it = rNNs->begin(); it != rNNs->end(); ++it)
    //   ofile<<(*it)->id<<endl;
    // rNNs->clear();
    // delete rNNs;

    total++;
  }

  AF_avg/=total;
  average_time/=total;
  average_time_true/=total;
  cout<<"AF_max= "<<AF_max<<"\tAF_avg= "<<AF_avg<<"\naverage_time=\t\t"<<average_time<<" nanoseconds\naverage_time_true=\t"<<average_time_true<<" nanoseconds\n";

  //------------------------------------clearing memmory
  ofile.close();
  data->clear();
  queries->clear();
  delete data;
  delete queries;
  return 0;
}
