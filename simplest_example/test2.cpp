#include <cstdio>
#include <assert.h>
#include <iostream>
using namespace std;
#include "vops.h"
#include "test2.h"
namespace ANONYMOUS{

void doubleSketch__Wrapper(int x) {
  doubleSketch(x);
}
void doubleSketch__WrapperNospec(int x) {}
void doubleSketch(int x) {
  int  t_s1=0;
  offByOne(x, t_s1);
  int  t=0;
  t = x * t_s1;
  assert ((t) == (((x * x) + x)));;
}
void offByOne(int x, int& _out) {
  _out = x + 1;
  return;
}

}
