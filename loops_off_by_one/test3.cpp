#include <cstdio>
#include <assert.h>
#include <iostream>
using namespace std;
#include "vops.h"
#include "test3.h"
namespace ANONYMOUS{

void add_one_to_each__Wrapper(int* x/* len = 5 */) {
  add_one_to_each(x);
}
void add_one_to_each__WrapperNospec(int* x/* len = 5 */) {}
void add_one_to_each(int* x/* len = 5 */) {
  int  i_s3=0;
  offByOne(0, i_s3);
  int  i=0;
  i = i_s3;
  int  __sa0_s5=0;
  offByOne2(4, __sa0_s5);
  bool  __sa0=0;
  __sa0 = (i_s3) < (__sa0_s5);
  bool _tt0[5] = {0, 0, 0, 0, 0};
  int*  next= new int [5]; CopyArr<int >(next,_tt0, 5, 5);
  while (__sa0) {
    (next[i]) = (x[i]) + 1;
    i = i + 1;
    int  __sa0_s7=0;
    offByOne2(4, __sa0_s7);
    __sa0 = (i) < (__sa0_s7);
  }
  for (int  i_0=0;(i_0) < (5);i_0 = i_0 + 1){
    assert (((next[i_0])) == (((x[i_0]) + 1)));;
  }
  delete[] next;
}
void offByOne(int x, int& _out) {
  _out = x;
  return;
}
void offByOne2(int x, int& _out) {
  _out = x + 1;
  return;
}

}
