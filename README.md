# Assignment 5: Autograder by Christopher Peterson

## What I did

### Sketch setup + Hello World

It seems like most of the heavy-lifting for this assignment was being done in Sketch, and getting any concrete results (other than python -> Sketch translation, which was an option but is ultimately nothing I haven't already done) would require its use.

I spent around an hour and a half installing Sketch and learning how it works. This was mostly challenging because I was using Windows, but also difficult due to the sparse documentation.

Using the following input file (modified from one found in https://people.csail.mit.edu/asolar/manual.pdf):
```
harness void doubleSketch(int x){
   int t = x * ??;
   assert t == x + x + x +x;
}
```

Sketch produced these C++ and H files respectively:
```
#include <cstdio>
#include <assert.h>
#include <iostream>
using namespace std;
#include "vops.h"
#include "test.h"
namespace ANONYMOUS{

void doubleSketch__Wrapper(int x) {
  doubleSketch(x);
}
void doubleSketch__WrapperNospec(int x) {}
void doubleSketch(int x) {
  assert (((x * 4)) == ((((x + x) + x) + x)));;
}

}
```
```
#ifndef TEST_H
#define TEST_H

#include <cstring>

#include "vops.h"

namespace ANONYMOUS{
}
namespace ANONYMOUS{
extern void doubleSketch__Wrapper(int x);
extern void doubleSketch__WrapperNospec(int x);
extern void doubleSketch(int x);
}

#endif
```

As someone who is new to using Sketch, this seemed like a strange (but technically correct) result. I was expecting the result to be in the same form as the original code, but with the ?? replaced. Seeing the code entirely remove the ``int t = x * ??`` line was concerning.
