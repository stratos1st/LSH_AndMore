#include <iostream>
#include <chrono>
#include <math.h>
#include <unordered_set>


#include "my_vector.hpp"
#include "my_curve.hpp"
#include "util.hpp"
#include "traversal_projection.hpp"


#define DEBUG 0

using namespace std;

int main(){
  
  traversal_projection a(3);
  a.print_big_table();

  return 0;
}
