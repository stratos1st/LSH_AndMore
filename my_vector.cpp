#include "my_vector.hpp"

#include <iostream>

using namespace std;

#define DEBUG 0

my_vector::my_vector(unsigned int dimentions){
  #if DEBUG
  cout<<"Constructing "<<dimentions<<"d vector"<<'\n';
  #endif
  coordinates=new double[dimentions];
  dim=dimentions;
  // for(unsigned int i=0;i<get_dimentions();i++)
  //   coordinates[i]=0.0;
}

my_vector::~my_vector(){
  #if DEBUG
  cout<<"Destructing "<<get_dimentions()<<"d vector"<<'\n';
  #endif
  delete[] coordinates;
}

my_vector& my_vector::operator=(const my_vector &other){
  if(get_dimentions()!=other.get_dimentions()){
    cerr<<"\n\n!! ERROR my_vector coppy construstor !!\n\n";
    exit(1);
  }

  for(unsigned int i=0;i<get_dimentions();i++)
    coordinates[i]=other.coordinates[i];
  id=other.id;

  return *this;
}

my_vector::my_vector(const my_vector &p2){
  #if DEBUG
  cout<<"Copy Constructing "<<p2.get_dimentions()<<"d vector"<<'\n';
  #endif
  coordinates=new double[p2.get_dimentions()];
  dim=p2.get_dimentions();

  // for(unsigned int i=0;i<p2.get_dimentions();i++)
  //   coordinates[i]=0.0;

  *this=p2;
}

unsigned int my_vector::get_dimentions() const{
  return dim;//sizeof(coordinates)/sizeof(coordinates[0]);
}

void my_vector::print_vec(unsigned int until){
  cout<<get_dimentions()<<"d vector "<<id<<" = ";
  if(until>get_dimentions()){
    cerr<<"!! ERROR print_vec";
    return;
  }
  if(until==0)
    until=get_dimentions();
  for(unsigned int i=0;i<until;i++)
    cout<<coordinates[i]<<", ";
  cout<<endl;
}

bool operator==(const my_vector &other,const my_vector &other2){
  if(other.get_dimentions()!=other2.get_dimentions())
    return false;
  for(unsigned int i=0;i<other.get_dimentions();i++)
    if (other2.coordinates[i]!=other.coordinates[i])
      return false;
  return true;
}
