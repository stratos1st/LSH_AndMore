#ifndef MY_VECTOR
#define MY_VECTOR

class my_vector{
 public:
  int *coordinates;
  unsigned int id;

  my_vector(unsigned int dimentions);
  ~my_vector();
  my_vector& operator=(const my_vector &other);//copies id
  my_vector(const my_vector &p2);//copies id

  unsigned int get_dimentions() const;
  void print_vec(unsigned int until=0);//prints x1,x1,...,xuntil. if until==0 prints all
};

#endif
