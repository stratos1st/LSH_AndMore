#include <iostream>
#include <fstream>
#include <chrono>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <bits/stdc++.h>

#include "my_vector.hpp"
#include "util.hpp"
#include "random_projection.hpp"

#define DEBUG 0

using namespace std;

int main(int argc, char *argv[]){
  //k is the k from g(x)
  //new_d is the d' from random projection aka k in arguments
  //w is the window in h
  float w=4000;
  int k=4,new_d=-1,max_points=INT_MAX,prodes=25,m=3;
  //w not needed by projectas argument. w should be float
  float r=1000;
  size_t f_container_sz=200,cube_container_size=9000;//not needed by project
  char input_file_data[100]("");//./.atomignore/input_small_id
  char input_file_queries[100]("");//./.atomignore/query_small_id
  char out_file[100]("cube_out");

  //------------------------------------parse arguments
  int opt;
  while((opt = getopt(argc, argv, "d:q:k:o:w:M:p:r:m:f:c"))!=-1){
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
        max_points=atoi(optarg);
        break;
      case 'p':
        prodes=atoi(optarg);
        break;
      case 'r':
        r=atof(optarg);
        break;
      case 'm'://-----
        m=atoi(optarg);
        break;
      case 'f'://-----
        f_container_sz=atoi(optarg);
        break;
      case 'c'://-----
        cube_container_size=atoi(optarg);
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
  list <my_vector> *data=read_vector_file(input_file_data);
  list <my_vector> *queries=read_vector_file(input_file_queries);

  //------------------------------------create and train model
  if(new_d==-1){
    new_d=log2(data->size());
    cout<<"default d'= "<<new_d<<endl;
  }
  random_projection_vector model(w,k,new_d,cube_container_size,f_container_sz,m);
  model.train(data);
  cout<<"cube training done!!\n";
  //------------------------------------loop

  char option = 'y';
  do{
    //------------------------------------info passed
    cout<<"program running with:\n\tdata_file= "<<input_file_data<<
      "\n\tquery_file= "<<input_file_queries<<"\n\tout_file= "<<out_file<<
      "\n\td'= "<<new_d<<"\n\tm= "<<m<<"\n\tM= "<<max_points<<"\n\tprodes= "<<prodes
      <<"\n\tw= "<<w<<"\n\tk= "<<k<<"\n\tr= "<<r<<"\n\tcube_container_sz= "<<
      cube_container_size<<"\n\tf_container_sz= "<<f_container_sz<<endl<<endl;
    //------------------------------------fill out file, running bruteNN and cubeNN
    double AF_max=0,AF_avg=0.0,AF;
    long int average_time=0;
    long int average_time_true=0;
    unsigned int total=0;
    using namespace std::chrono;
    for(list <my_vector> :: iterator it = queries->begin(); it != queries->end(); ++it){
      auto start = high_resolution_clock::now();
      pair<my_vector*,double> nn_brute=brute_NN(data,*it,manhattan_distance);
      auto stop = high_resolution_clock::now();
      auto duration_brute = duration_cast<nanoseconds>(stop - start);

      start = high_resolution_clock::now();
      pair<my_vector*,double> nn_cube=model.find_NN(*it,manhattan_distance,max_points,prodes);
      stop = high_resolution_clock::now();
      auto duration_lsh = duration_cast<nanoseconds>(stop - start);

      AF=nn_cube.second/nn_brute.second;
      AF_max=max(AF,AF_max);
      AF_avg+=AF;
      average_time_true+=duration_brute.count();
      average_time+=duration_lsh.count();

      ofile<<"Query: "<<it->id<<endl;
      ofile<<"Nearest neighbor: "<<nn_cube.first->id<<endl;
      ofile<<"distanceCube: "<<nn_cube.second<<endl;
      ofile<<"distanceTrue: "<<nn_brute.second<<endl;
      ofile<<"tCube: "<<duration_lsh.count()<<endl;
      ofile<<"tTrue: "<<duration_brute.count()<<endl;
      ofile<<"R-near neighbors: "<<endl;

      list<my_vector*>* rNNs=model.find_rNN(*it,r,manhattan_distance,max_points,prodes);
      for(list <my_vector*>::iterator it = rNNs->begin(); it != rNNs->end(); ++it)
        ofile<<(*it)->id<<endl;
      rNNs->clear();
      delete rNNs;

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
    //------------------------------------change input

    std::cout << "Would you like to run Cube for another queryset (y/n)" << '\n';
    std::cin >> option;
    while(option != 'y' && option != 'n')
      std::cin >> option;
    if(option == 'y'){
      std::cout << "Enter new queryset path" << '\n';
      std::cin >> input_file_queries;
      queries->clear();
      delete queries;
      queries=read_vector_file(input_file_queries);
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
