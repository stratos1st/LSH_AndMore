#include <iostream>
#include <fstream>
#include <chrono>
#include <unistd.h>
#include <string.h>

#include "my_curve.hpp"
#include "util.hpp"
#include "lsh.hpp"

#define DEBUG 0

#define MAX_CURVE_POINTS 50

using namespace std;

int main(int argc, char *argv[]){
  //w is the window in h
  int k=4, l=5,w=4000,m=3;//w not needed by project as argument. w should be float
  size_t lsh_container_size=9000;//not needed by project

  char input_file_data[100]("./.atomignore/trajectories_dataset_cut");
  char input_file_queries[100]("./.atomignore/queriecurve");
  char out_file[100]("lsh_curves_out");
  double r=0.0001;

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
  cout<<"program running with:\n\tdata_file= "<<input_file_data<<
    "\n\tquery_file= "<<input_file_queries<<"\n\tout_file= "<<out_file<<
    "\n\tk= "<<k<<"\n\tl= "<<l<<"\n\tw= "<<w<<"\n\tm= "<<m<<"\n\tr= "<<r
    <<"\n\tf_container_sz= "<<lsh_container_size<<endl<<endl;

  //------------------------------------create out file
  ofstream ofile;
  ofile.open(out_file);
  if(!ofile.is_open()){
    cout<<"!! ERROR OUTFILE !!\n";
    exit(1);
  }

  //------------------------------------read input files
  list <my_curve> *data1=read_curve_file(input_file_data);
  list <my_curve> *data=new list <my_curve>;
  for(auto it=data1->begin();it!=data1->end();++it)
    if(it->numofvectors<=MAX_CURVE_POINTS)
      data->push_back(*it);

  list <my_curve> *queries1=read_curve_file(input_file_queries);
  list <my_curve> *queries=new list <my_curve>;
  for(auto it=queries1->begin();it!=queries1->end();++it)
    if(it->numofvectors<=MAX_CURVE_POINTS)
      queries->push_back(*it);
  cout<<"read files done\n";

  //------------------------------------create and train model
  lsh_curve lsh_model(data->front().vectors[0]->get_dimentions(),MAX_CURVE_POINTS,l,w,k,lsh_container_size,m);
  lsh_model.train(data);
  cout<<"lsh training done!!\n";

  //------------------------------------fill out file, running bruteNN and cubeNN
  double AF_max=0.0,AF_avg=0.0,AF;
  long int average_time=0;
  unsigned int total=0;
  using namespace std::chrono;
  for(list <my_curve> :: iterator it = queries->begin(); it != queries->end(); ++it){
    auto start = high_resolution_clock::now();
    pair<my_curve*,double> nn_brute=brute_NN_curve(data,*it,Dtw);
    auto stop = high_resolution_clock::now();
    auto duration_brute = duration_cast<nanoseconds>(stop - start);

    start = high_resolution_clock::now();
    pair<my_curve*,double> nn_lsh=lsh_model.find_NN(*it,Dtw);
    stop = high_resolution_clock::now();
    auto duration_lsh = duration_cast<nanoseconds>(stop - start);

    if(nn_brute.second==0.0){
      cout<<"Warining: distance is ~=0! querry does not count\n";
      total--;
    }
    else
      AF=nn_lsh.second/nn_brute.second;
    AF_max=max(AF,AF_max);
    AF_avg+=AF;
    average_time+=duration_lsh.count();

    ofile<<"Query: "<<it->id<<endl;
    ofile<<"Nearest neighbor: "<<nn_lsh.first->id<<endl;
    ofile<<"distanceLSH: "<<nn_lsh.second<<endl;
    ofile<<"distanceTrue: "<<nn_brute.second<<endl;
    ofile<<"tLSH: "<<duration_lsh.count()<<endl;
    ofile<<"tTrue: "<<duration_brute.count()<<endl;
    ofile<<"R-near neighbors: "<<endl;

    list<my_curve*>* rNNs=lsh_model.find_rNN(*it,r,Dtw);
    for(list <my_curve*>::iterator it = rNNs->begin(); it != rNNs->end(); ++it)
      ofile<<(*it)->id<<endl;
    rNNs->clear();
    delete rNNs;

    total++;
  }

  AF_avg/=total;
  average_time/=total;
  cout<<"AF_max= "<<AF_max<<"\tAF_avg= "<<AF_avg<<"\taverage_time= "<<average_time<<" nanoseconds\n";

  //------------------------------------clearing memmory
  ofile.close();
  data->clear();
  queries->clear();
  delete data;
  delete queries;

  return 0;
}
