#include <cstdio>
#include <assert.h>
#include <iostream>
using namespace std;
#include "vops.h"
#include "test5.h"
namespace ANONYMOUS{

void arbitrary_length__Wrapper(int n, int* x/* len = n */) {
  arbitrary_length(n, x);
}
void arbitrary_length__WrapperNospec(int n, int* x/* len = n */) {}
void arbitrary_length(int n, int* x/* len = n */) {
  if ((n) == (0)) {
    return;
  }
  int  testLen=0;
  int*  test_s6= new int [n]; CopyArr<int >(test_s6,0, n);
  student_solution(n, x, testLen, test_s6);
  int  actualLen=0;
  int*  actual_s8= new int [n]; CopyArr<int >(actual_s8,0, n);
  reference_solution(n, x, actualLen, actual_s8);
  assert ((testLen) == (actualLen));;
  for (int  i=0;(i) < (actualLen);i = i + 1){
    assert (((test_s6[i])) == ((actual_s8[i])));;
  }
  delete[] test_s6;
  delete[] actual_s8;
}
void student_solution(int n, int* x/* len = n */, int& output_len, int* _out/* len = n */) {
  output_len = 0;
  bool  _out_s10=0;
  condition1((n) == (1), _out_s10);
  int*  output= new int [n]; CopyArr<int >(output,0, n);
  if (_out_s10) {
    int*  _out_s12= new int [n]; CopyArr<int >(_out_s12,0, n);
    returnValue1(n, output, output_len, _out_s12);
    CopyArr<int >(_out,_out_s12, n, n);
    delete[] _out_s12;
  delete[] output;
    return;
  }
  int  i_s14=0;
  rangeLowerValue(0, i_s14);
  int  i=0;
  i = i_s14;
  bool  __sa0=(i_s14) < (n);
  int  appendIndex=0;
  while (__sa0) {
    bool  _out_s16=0;
    condition2(((x[i])) == (0), _out_s16);
    if (_out_s16) ; else {
      int  uo_s2=appendIndex;
      appendIndex = appendIndex + 1;
      (output[uo_s2]) = (x[i]) * i;
      output_len = output_len + 1;
    }
    i = i + 1;
    __sa0 = (i) < (n);
  }
  int*  _out_s18= new int [n]; CopyArr<int >(_out_s18,0, n);
  returnValue2(n, output, output_len, _out_s18);
  CopyArr<int >(_out,_out_s18, n, n);
  delete[] output;
  delete[] _out_s18;
  return;
}
void reference_solution(int n, int* x/* len = n */, int& output_len, int* _out/* len = n */) {
  CopyArr<int >(_out,0, n);
  if ((n) == (1)) {
    (_out[0]) = 0;
    output_len = 1;
    return;
  }
  for (int  i=1;(i) < (n);i = i + 1){
    (_out[i - 1]) = (x[i]) * i;
  }
  output_len = n - 1;
  return;
}
void condition1(bool x, bool& _out) {
  _out = x;
  return;
}
void returnValue1(int n, int* x_0/* len = n */, int& return_length, int* _out/* len = n */) {
  int*  x= new int [n]; CopyArr<int >(x,x_0, n, n);
  (x[0]) = 0;
  return_length = 1;
  CopyArr<int >(_out,x, n, n);
  delete[] x;
  return;
}
void rangeLowerValue(int x, int& _out) {
  _out = x + 1;
  return;
}
void condition2(bool x, bool& _out) {
  _out = 0;
  return;
}
void returnValue2(int n, int* x_0/* len = n */, int& return_length, int* _out/* len = n */) {
  CopyArr<int >(_out,x_0, n, n);
  return;
}

}
