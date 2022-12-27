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


using namespace std;

int main(int argc, char *argv[]){
    //w is the window in h
    int k=4,l=5,m=999999,max_curve_points=5;
    size_t lsh_container_size=100;//not needed by project
    float w=0.00009;
    double e = 0.5;
    unsigned int how_many = 2,window =2;

    char input_file_data[100]("");
    char input_file_queries[100]("");
    char out_file[100]("curve_projection_lsh_out");

    //------------------------------------parse arguments
    int opt;
    while((opt = getopt(argc, argv, "d:q:k:L:o:w:m:c:e:s:h:n:"))!=-1){
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
              w=atof(optarg);
              break;
            case 'm':
              m=atoi(optarg);
              break;
            case 'c':
              lsh_container_size=atoi(optarg);
              break;
            case 'e':
              e=atof(optarg);
              break;
            case 's':
              max_curve_points=atoi(optarg);
              break;
            case 'h':
              how_many=atoi(optarg);
              break;
            case 'W':
              window=atoi(optarg);
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

              // cout.setstate(ios::failbit);
              //------------------------------------read input files
              list <my_curve> *data=read_curve_file(input_file_data,max_curve_points);
              list <my_curve> *queries=read_curve_file(input_file_queries,max_curve_points);
              cout<<"read files done\n";

              //------------------------------------create and train model
              traversal_projection_lsh lsh_model(max_curve_points,how_many);
              lsh_model.train(data,e,l,w,k,lsh_container_size,99999.99999,m);//pad erelevant
              cout<<"traversal_projection<lsh_curve> training done!!\n";

              // cout.clear();
              char option = 'y';
              do{
                  cout<<"program running with:\n\tdata_file= "<<input_file_data<<
                    "\n\tquery_file= "<<input_file_queries<<"\n\tout_file= "<<out_file
                    <<"\n\tk= "<<k
                    <<"\n\tL= "<<l
                    <<"\n\tw= "<<w
                    <<"\n\tm= "<<m
                    <<"\n\ts= "<<max_curve_points
                    <<"\n\tf_container_sz= "<<lsh_container_size
                    <<"\n\te= "<<e
                    <<"\n\thow_many= "<<how_many
                    <<"\n\twindow= "<<window
                    <<endl<<endl;
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
                      pair<my_curve*,double> nn_lsh=lsh_model.find_NN(*it,window,Dtw,manhattan_distance);
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
                          ofile<<"Method: Projection\nHashFunction LSH";
                          ofile<<"Found Nearest neighbor: "<<nn_lsh.first->id<<endl;
                          ofile<<"True Nearest neighbor: "<<nn_brute.first->id<<endl;
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

                          //------------------------------------change input

                          std::cout << "Would you like to run LSH for another queryset (y/n)" << '\n';
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
