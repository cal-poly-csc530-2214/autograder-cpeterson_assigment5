#include <cstdio>
#include <assert.h>
#include <iostream>
using namespace std;
#include "vops.h"
#include "test4.h"
namespace ANONYMOUS{

void arbitrary_length__Wrapper(int n, int* x/* len = n */) {
  arbitrary_length(n, x);
}
void arbitrary_length__WrapperNospec(int n, int* x/* len = n */) {}
void arbitrary_length(int n, int* x/* len = n */) {
  int*  test_s3= new int [n]; CopyArr<int >(test_s3,0, n);
  student_solution(n, x, test_s3);
  int*  actual_s5= new int [n]; CopyArr<int >(actual_s5,0, n);
  reference_solution(n, x, actual_s5);
  for (int  i=0;(i) < (n);i = i + 1){
    assert (((test_s3[i])) == ((actual_s5[i])));;
  }
  delete[] test_s3;
  delete[] actual_s5;
}
void student_solution(int n, int* x/* len = n */, int* _out/* len = n */) {
  CopyArr<int >(_out,0, n);
  int  i_s7=0;
  offByOne(1, i_s7);
  int  i=0;
  i = i_s7;
  bool  __sa0=(i_s7) < (n);
  while (__sa0) {
    int  _out_s9=0;
    offByOne2(1, _out_s9);
    int  _out_s11=0;
    correctOperator((x[i]), _out_s9, _out_s11);
    (_out[i]) = _out_s11;
    i = i + 1;
    __sa0 = (i) < (n);
  }
  return;
}
void reference_solution(int n, int* x/* len = n */, int* _out/* len = n */) {
  CopyArr<int >(_out,0, n);
  for (int  i=0;(i) < (n);i = i + 1){
    (_out[i]) = (x[i]) + 1;
  }
  return;
}
void offByOne(int x, int& _out) {
  _out = x - 1;
  return;
}
void offByOne2(int x, int& _out) {
  _out = x;
  return;
}
void correctOperator(int x, int y, int& _out) {
  _out = x + y;
  return;
}

}
