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

This program is essentially asking Sketch if the second ``x`` of the student's code ``int t = x * x`` should be replaced with ``x + 1``. The answer to this question is obviously yes, which Sketch is able to find:
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

### Checking for loop off-by-one errors and adding more corrections

I wanted to apply what I had learned to something that could actually be useful and is a common error new programmers make: automatically checking for off-by-one errors in loop bounds. To do this, I used the previous offByOne error definitions applied to a program that adds one to each element in a loop:
```
harness void add_one_to_each(int[5] x){
   // user code
   int[5] next;
   for (int i = offByOne(0); i < offByOne2(4); i++){
      next[i] = x[i] + 1;
   }

   // check correctness
   for (int i = 0; i < 5; i++){
      assert next[i] == x[i] + 1;
   }
}

int offByOne(int x){
   if (??) return x;

   return x + 1;
}

int offByOne2(int x){
   if (??) return x;

   return x + 1;
}
```

Sparing the output file copy-paste, this works as expected. After being run through the Sketch solver, offByOne uses the originally supplied version (since a lower bound of 0 is correct), and offByOne2 uses the corrected version (since the upper bound should be 5).

The next thing I wanted to do (although the paper doesn't go into a ton of detail about this, since they have the Python -> C++ conversion to worry about) is add multiple different corrections to the same original piece of code. In this case, I added both +1 and -1 as possible corrections to an off-by-one issue:
```
harness void add_one_to_each(int[5] x){
   // user code
   int[5] next;
   for (int i = offByOne(1); i <= offByOne2(5); i++){
      next[i] = x[i] + 1;
   }

   // check correctness
   for (int i = 0; i < 5; i++){
      assert next[i] == x[i] + 1;
   }
}

int offByOne(int x){
   if (??) return x;
   if (??) return x - 1;

   return x + 1;
}

int offByOne2(int x){
   if (??) return x;
   if (??) return x - 1;

   return x + 1;
}
```

It's reasonably easy to see that both offByOne's should return the ``x - 1`` version of their output, since the loop should start at 0 and stop at 4. The Sketch solver finds this answer, as well.

### Generalized format, arbitrary list sizes, more errors

Before implementing the author's example (polynomial derivatives), I had to add a few more pieces to my analysis. This performs the same operation as before (adding one to each element of a list), but is a little more generalized. Additionally, I've added an error case to check if the user is using the right operator. This was surprisingly annoying to get right with the lack of Sketch documentation, but eventually I realized it works when ``--bnd-unroll-amnt`` is set to a very large number:
```
harness void arbitrary_length(int n, int[n] x){
   // get results
   int[n] test = student_solution(n, x);
   int[n] actual = reference_solution(n, x);

   // check correctness
   for (int i = 0; i < n; i++){
      assert test[i] == actual[i];
   }
}

// correct version
int[n] reference_solution(int n, int[n] x){
   int[n] results;
   for (int i = 0; i < n; i++){
      results[i] = x[i] + 1;
   }
   
   return results;
}

// student version
int[n] student_solution(int n, int[n] x){
   int[n] results;
   for (int i = offByOne(1); i < n; i++){
      results[i] = correctOperator(x[i], offByOne2(1));
   }
   
   return results;
}

//possible errors
int offByOne(int x){
   if (??) return x;
   if (??) return x - 1;

   return x + 1;
}

int offByOne2(int x){
   if (??) return x;
   if (??) return x - 1;

   return x + 1;
}

int correctOperator(int x, int y){
   if (??) return x - y;
   if (??) return x + y;
   if (??) return x * y;
   
   return x / y;
}
```

As expected, offByOne uses the ``x - 1`` version, correctOperator uses the ``+`` version, and offByOne2 requires no change. Interesetingly, this code takes a bit longer to run than previous tests, which makes sense as it's a bit more complicated.
