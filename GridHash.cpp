#include "GridHash.hpp"

using namespace std;

my_vector& GridHash::gridify(my_curve& c){// maybe call it vectorize
  if (c.vectordimentions != t->dim) {
    cerr<<"\n\n!! ERROR this curve has dif dimentions from the shift vec !!\n\n";
    exit(1);
  }
  //snap - map point to nearest grid point
  list <my_vector> gridcurve;//this list will be used later for the merge of same vectors
  for(unsigned int i=0;i<=c.numofvectors-1;i++){
    my_vector* newpoint = new my_vector(*c.vectors[i]);// make a new copy of the vector that will be saved in the list
    for(unsigned int j=0;j<=c.vectordimentions-1;j++)
    {
      double x=c.vectors[i]->coordinates[j];
      double shift = x-t->coordinates[j]; // i am shifting temporarily allthe points from the opposite direction istead of the curve being shifted
      newpoint[j]==floor(shift/delta)*delta+(fmod(shift,delta)>delta/2)*delta+t->coordinates[j];
    }
    gridcurve.push_back(*newpoint);
  }
  // remove dublicates
  list <my_vector> :: iterator previous = gridcurve.begin();
  for(list <my_vector> :: iterator it = gridcurve.begin()++; it != gridcurve.end(); ++it){
    if (*previous==*it) {// might be wrong
      gridcurve.erase(it);//maybe needs it++/++it
    }
  }
  //concate points
  my_vector* vectorcurve = new my_vector(gridcurve.size()*c.vectordimentions);
  unsigned int i = 0;
  for(list <my_vector> :: iterator it = gridcurve.begin(); it != gridcurve.end(); ++it){
    for (unsigned int j = 0; j <= c.vectordimentions; j++) {
      vectorcurve->coordinates[i++]=it->coordinates[j];
    }
  }
  return *vectorcurve;
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
