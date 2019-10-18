#include <iostream>
#include <limits>
#include "util.hpp"
#include "my_curves.hpp"
using namespace std;

float Dtw( my_curve& x, my_curve& y)
{
  int n=10,m=10;
  float OptValue [n+1][m+1];  // where the value of Dtw will be stored

  // initializing outer shells of OptValue with infinity
  OptValue[0][0]=0;
  for(int i=1;i<=n;i++)
    OptValue[i][0]=numeric_limits<float>::infinity();
  for(int i=1;i<=m;i++)
    OptValue[0][i]=numeric_limits<float>::infinity();
  //implementing the iterative algorithm to fill OptValue
  for(int i=1;i<=n;i++)
    for(int j=1;j<=m;j++)
      OptValue[i][j]=min(min(OptValue[i][j-1],OptValue[i-1][j]),OptValue[i-1][j-1])+manhattan_distance(x.get_vector(i), y.get_vector(j));
  //manhattan_distance is iterchangable with other norms
  //TODO return 1 opt solution
  return OptValue[n][m];
}
