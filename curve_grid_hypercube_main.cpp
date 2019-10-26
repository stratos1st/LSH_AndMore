#include <iostream>
#include <fstream>
#include <chrono>
#include <unistd.h>
#include <string.h>
#include <bits/stdc++.h>

#include "my_curve.hpp"
#include "util.hpp"
#include "random_projection.hpp"

#define DEBUG 0

using namespace std;

int main(int argc, char *argv[]){
  //k is the k from g(x)
  //new_d is the d' from random projection aka k in arguments
  //w is the window in h
  int k=4,new_d=-1,max_curve_points=0,prodes=20,m=150;
  float w=0.001;
  //w not needed by projectas argument. w should be float
  //double r=0.0001;
  size_t f_container_sz=200,cube_container_size=100;//not needed by project

  char input_file_data[100]("");
  char input_file_queries[100]("");
  char out_file[100]("curve_grid_hypercube_out");

  //------------------------------------parse arguments
  int opt;
  while((opt = getopt(argc, argv, "d:q:k:o:w:M:p:r:m:f:c:D:"))!=-1){
    switch(opt){
      case 'd':
        cout<<optarg<<endl;
        strcpy(input_file_data,optarg);
        break;
      case 'q':
        strcpy(input_file_queries,optarg);
        break;
      case 'k':
        new_d=atoi(optarg);
        break;
      case 'o':
        strcpy(out_file,optarg);
        break;
      case 'w':
        w=atof(optarg);
        break;
      case 'M':
        max_curve_points=atoi(optarg);
        break;
      case 'p':
        prodes=atoi(optarg);
        break;
      // case 'r':
      //   r=atof(optarg);
      //   break;
      case 'm':
        m=atoi(optarg);
        break;
      case 'f':
        f_container_sz=atoi(optarg);
        break;
      case 'c':
        cube_container_size=atoi(optarg);
        break;
      case 'D':
        GridHash::delta=atof(optarg);
        break;
      default:
        cout<<"!! WRONG ARGUMENTS !!\n";
        exit(1);
    }
  }
  //------------------------------------reading from keyboard

  if(input_file_data[0]=='\0'){ // if it is the empty string promt
    std::cout << "Enter dataset path" << '\n';
    std::cin >> input_file_data;
  }
  if(input_file_queries[0]=='\0'){ // if it is the empty string promt
    std::cout << "Enter queryset path" << '\n';
    std::cin >> input_file_queries;
  }


  //------------------------------------create out file
  ofstream ofile;
  ofile.open(out_file);
  if(!ofile.is_open()){
    cout<<"!! ERROR OUTFILE !!\n";
    exit(1);
  }

  //------------------------------------read input files
  list <my_curve> *data=read_curve_file(input_file_data,max_curve_points);
  list <my_curve> *queries=read_curve_file(input_file_queries,max_curve_points);
  if(new_d==-1){
    new_d=log2(data->size());
    cout<<"default d'= "<<new_d<<endl;
  }
  cout<<"reading files done!!\n";

  //------------------------------------create and train model
  random_projection_curve cube_model(max_curve_points,w,k,new_d,99999.99999,cube_container_size,f_container_sz,m);
  cube_model.train(data);
  cout<<"random_projection_curve training done!!\n";
  //------------------------------------loop

  char option = 'y';
  do{
    //------------------------------------info passed
    cout<<"program running with:\n\tdata_file= "<<input_file_data<<
      "\n\tquery_file= "<<input_file_queries<<"\n\tout_file= "<<out_file<<
      "\n\td'= "<<new_d<<"\n\tm= "<<m<<"\n\tM= "<<max_curve_points<<"\n\tprodes= "<<prodes
      <<"\n\tw= "<<w<<"\n\tk= "<<k<</*"\n\tr= "<<r<<*/"\n\tcube_container_sz= "<<
      cube_container_size<<"\n\tf_container_sz= "<<f_container_sz<<"\n\tdelta= "<<GridHash::delta<<endl<<endl;
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
      pair<my_curve*,double> nn_cube_gridcurves=cube_model.find_NN(*it,Dtw,manhattan_distance);
      stop = high_resolution_clock::now();
      auto duration_lsh = duration_cast<nanoseconds>(stop - start);

      if(nn_brute.second==0.0){
        cout<<"Warining: real distance is ~=0! querry "<<it->id<<" does not count\n";
        continue;
      }
      if(nn_cube_gridcurves.second==DBL_MAX){
        cout<<"Warining: predicted distance is ~=inf! querry "<<it->id<<" does not count\n";
        continue;
      }

      AF=nn_cube_gridcurves.second/nn_brute.second;
      AF_max=max(AF,AF_max);
      AF_avg+=AF;
      average_time+=duration_lsh.count();
      average_time_true+=duration_brute.count();

      ofile<<"Query: "<<it->id<<endl;
      ofile<<"Nearest neighbor: "<<nn_cube_gridcurves.first->id<<endl;
      ofile<<"distanceCube: "<<nn_cube_gridcurves.second<<endl;
      ofile<<"distanceTrue: "<<nn_brute.second<<endl;
      ofile<<"tCube: "<<duration_lsh.count()<<endl;
      ofile<<"tTrue: "<<duration_brute.count()<<endl;
      //ofile<<"R-near neighbors: "<<endl;

      // list<my_curve*>* rNNs=cube_model.find_rNN(*it,r,Dtw);
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
    //------------------------------------change input

    std::cout << "Would you like to run Hypercube_gridcurves for another queryset (y/n)" << '\n';
    std::cin >> option;
    while(option != 'y' && option != 'n')
      std::cin >> option;
    if(option == 'y'){
      std::cout << "Enter new queryset path" << '\n';
      std::cin >> input_file_queries;
      queries->clear();
      delete queries;
      queries=read_curve_file(input_file_queries,max_curve_points);
    }
  }while (option == 'y');
  //------------------------------------clearing memmory
  ofile.close();
  data->clear();
  queries->clear();
  delete data;
  delete queries;
  return 0;
}
