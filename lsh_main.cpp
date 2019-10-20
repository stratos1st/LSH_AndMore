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
  // unsigned int m=pow(2,32)-5;//could include m in arguments. tihs is the default value for m
  //w is the window in h
  int k=4, l=5,w=4000;//w not needed by project. w should be float
  char input_file_data[100]("./.atomignore/input_small_id");
  char input_file_queries[100]("./.atomignore/query_small_id");
  char out_file[100]("lsh_out");
  double r=1000;

  //------------------------------------parse arguments
  int opt;
  while((opt = getopt(argc, argv, "d:q:k:L:o:w:r:"))!=-1){
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
      default:
        cout<<"!! WRONG ARGUMENTS !!\n";
        exit(1);
    }
  }
  cout<<"program running with:\n\tdata_file= "<<input_file_data<<
    "\n\tquery_file= "<<input_file_queries<<"\n\tout_file= "<<out_file<<
    "\n\tk= "<<k<<"\n\tl= "<<l<<"\n\tw= "<<w<<"\n\tr= "<<r<<endl<<endl;

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
  cout<<"read files done\n";

  //------------------------------------create and train model
  lsh lsh_model(data->front().get_dimentions(), l, w, k);
  lsh_model.train(data);
  cout<<"lsh training done!!\n";

  //------------------------------------fill out file, running bruteNN and cubeNN
  double AF_max=0.0,AF_avg=0.0,AF;
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
  cout<<"AF_max= "<<AF_max<<"\tAF_avg= "<<AF_avg<<endl;

  //------------------------------------clearing memmory
  ofile.close();
  data->clear();
  queries->clear();
  delete data;
  delete queries;

  return 0;
}
