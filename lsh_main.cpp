#include <iostream>
#include <fstream>
#include <chrono>
#include <unistd.h>
#include <string.h>

#include "my_vector.hpp"
#include "util.hpp"
#include "lsh.hpp"

#define DEBUG 0

using namespace std;

int main(int argc, char *argv[]){
  //w is the window in h
  float w=4000;
  int k=4, l=5,m=3;//w not needed by project as argument. w should be float
  size_t lsh_container_size=9000;//not needed by project

    char input_file_data[100]("");//./.atomignore/input_small_id
  char input_file_queries[100]("");//./.atomignore/query_small_id
  char out_file[100]("lsh_out");
  double r=1000;

  //------------------------------------parse arguments
  int opt;
  while((opt = getopt(argc, argv, "d:q:k:L:o:w:r:m:c:"))!=-1){
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
  lsh_vector lsh_model(data->front().get_dimentions(),l,w,k,lsh_container_size,m);
  lsh_model.train(data);
  cout<<"lsh training done!!\n";

  //-----------------------------------loop
  char option = 'y';
  do {

    //------------------------------------info passed
    cout<<"program running with:\n\tdata_file= "<<input_file_data<<
      "\n\tquery_file= "<<input_file_queries<<"\n\tout_file= "<<out_file<<
      "\n\tk= "<<k<<"\n\tl= "<<l<<"\n\tw= "<<w<<"\n\tm= "<<m<<"\n\tr= "<<r
      <<"\n\tf_container_sz= "<<lsh_container_size<<endl<<endl;
    //------------------------------------fill out file, running bruteNN and cubeNN

    double AF_max=0.0,AF_avg=0.0,AF;
    long int average_time=0,average_time_true=0;
    unsigned int total=0;
    using namespace std::chrono;
    for(list <my_vector> :: iterator it = queries->begin(); it != queries->end(); ++it){
      auto start = high_resolution_clock::now();
      pair<my_vector*,double> nn_brute=brute_NN(data,*it,manhattan_distance);
      auto stop = high_resolution_clock::now();
      auto duration_brute = duration_cast<nanoseconds>(stop - start);

      start = high_resolution_clock::now();
      pair<my_vector*,double> nn_lsh=lsh_model.find_NN(*it,manhattan_distance);
      stop = high_resolution_clock::now();
      auto duration_lsh = duration_cast<nanoseconds>(stop - start);

      AF=nn_lsh.second/nn_brute.second;
      AF_max=max(AF,AF_max);
      AF_avg+=AF;
      average_time_true+=duration_brute.count();
      average_time+=duration_lsh.count();

      ofile<<"Query: "<<it->id<<endl;
      ofile<<"Nearest neighbor: "<<nn_lsh.first->id<<endl;
      ofile<<"distanceLSH: "<<nn_lsh.second<<endl;
      ofile<<"distanceTrue: "<<nn_brute.second<<endl;
      ofile<<"tLSH: "<<duration_lsh.count()<<endl;
      ofile<<"tTrue: "<<duration_brute.count()<<endl;
      ofile<<"R-near neighbors: "<<endl;

      list<my_vector*>* rNNs=lsh_model.find_rNN(*it,r,manhattan_distance);
      for(list <my_vector*>::iterator it = rNNs->begin(); it != rNNs->end(); ++it)
        ofile<<(*it)->id<<endl;
      rNNs->clear();
      delete rNNs;

      total++;
    }

    AF_avg/=total;
    average_time/=total;
    average_time_true/=total;

    cout<<"AF_max= "<<AF_max<<"\tAF_avg= "<<AF_avg<<"\naverage_time=\t\t"<<average_time<<" nanoseconds\navarage_time_true=\t"<<average_time_true<<" nanoseconds\n";

    //------------------------------------change input

    std::cout << "Would you like to run LSH for another queryset (y/n)" << '\n';
    std::cin >> option;
    while(option != 'y' && option != 'n')
      std::cin >> option;
    if(option == 'y'){
      std::cout << "Enter new queryset path" << '\n';
      std::cin >> input_file_queries;
      //cleaning memmory
      queries->clear();
      delete queries;
      queries=read_vector_file(input_file_queries);
    }
  }while(option == 'y');
  //------------------------------------clearing memmory
  ofile.close();
  data->clear();
  queries->clear();
  delete data;
  delete queries;

  return 0;
}
