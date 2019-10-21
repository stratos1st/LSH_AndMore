#include "GridHash.hpp"

using namespace std;

my_curve& GridHash::gridify(my_curve& c){
  if (c.vectordimentions != t->dim) {
    cerr<<"\n\n!! ERROR this curve has dif dimentions from the shift vec !!\n\n";
    exit(1);
  }
  my_curve* gridcurve = new my_curve(c.numofvectors,c.vectordimentions);//new empty vector
  //snap - map point to nearest grid point
  for(unsigned int i=0;i<=c.numofvectors-1;i++)
    for(unsigned int j=0;j<=c.vectordimentions-1;j++)
    {
      double x=c.vectors[i]->coordinates[j];
      double shift = x-t->coordinates[j]; // i am shifting temporarily allthe points from the opposite direction istead of the curve being shifted
      //gridcurve.vectors[i]->coordinates[j]=(shift/delta)*delta+(shift%delta>delta/2)*delta+t->coordinates[j];
    }
  return *gridcurve;
}

GridHash::GridHash(unsigned int dimentions){
  //randomly generate t vector
  // Initializing of uniform_real_distribution class
  t=new my_vector(dimentions);
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  default_random_engine generator (seed);
  uniform_real_distribution<double> distribution(0, delta);
  for (unsigned int i = 0; i <= dimentions; i++) {
    double shift = distribution(generator);
    while (shift == delta)  // go on till the rang value is in the [0,delta)
      shift = distribution(generator);
    t->coordinates[i]=shift;
  }
}

GridHash::GridHash(my_vector& c){
  for (unsigned int i = 0; i <= c.get_dimentions()-1; i++)
    if (c.coordinates[i]>= delta) {
      cerr<<"\n\n!! ERROR t is not a shift vector !!\n\n";
      exit(1);
  }
  t=new my_vector(c);
}

GridHash::~GridHash(){
  delete t;
}
