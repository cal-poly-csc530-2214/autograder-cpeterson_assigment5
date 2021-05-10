# Assignment 5: Autograder by Christopher Peterson

## What I did

I focused the vast majority of my time on C++ program grading using the technique proposed by the authors. I thought the Python multitype conversion stuff was interesting, but tangential to the core concepts of the paper. With my limited time, I chose to explore these in more detail.

### Sketch setup + Hello World

It seems like most of the heavy-lifting for this assignment was being done in Sketch, and getting any concrete results (other than Python -> Sketch translation, which was an option but is ultimately nothing I haven't already done) would require its use.

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

### Simplest autograded example

After working out the basics of Sketch syntax, I created something that began to resemble the form found in the paper:
```
harness void doubleSketch(int x){
   int t = x * offByOne(x);

   assert t == x * x + x;
}

int offByOne(int x){
   if (??) return x;

   return x + 1;
}
```

This program is essentially asking Sketch if the second x of the student's code ``int t = x * x`` should be replaced with ``x + 1``. The answer to this question is obviously yes, which Sketch is able to find:
```
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
```

The function offByOne uses the ``x + 1`` output, as expected. This output file looks like it'd take a lot of extra time investment to automatically parse, so I'll be sticking to increasingly complex manual observations.
