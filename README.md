# Assignment 5: Autograder by Christopher Peterson

## Contents <a name="contents"></a>
- [What I did](#what)
    - [Sketch setup + Hello World](#setup)
    - [Simplest autograded example](#simplest)
    - [Checking for loop off-by-one errors and adding more corrections](#offbyone)
    - [Generalized format, arbitrary list sizes, more errors](#arbitrary)
    - [Validating the author's results on computeDeriv submission A](#validating)
- [Conclusion](#conclusion)
- [Future work](#future)

## What I did <a name="what"></a>

I focused my efforts on examining the "Program Rewriter", "Sketch Translator", and "Sketch Solver" phases of the author's process (going from a reference program, student program, and list of possible errors to a corrected program). This seemed like the most interesting section of the analysis and also the best covered by our lectures.

Instead of using Python programs, I examined native C++ ones. This allowed me to avoid all of the multi-type conversion overhead and focus my efforts on actually generating interesting conclusions/validating the author's claims. I didn't think much was *lost* by limiting my scope to autograding C++ programs (it's almost the same exact problem, just much more straightforward), so this seemed like an excellent simplification.

In the approximately 5 hours I spent on this project, I:
- Downloaded, installed, and learned how to use Sketch on my Windows system
- Manually ran through the autograding parsing steps to get Sketch solver-ready versions of several progressively more complex example problems
- Validated the author's exact process and results for Figure 2, student submission A
- Understood the steps needed to further automate the process or modify it to work on Python programs

### Sketch setup + Hello World <a name="setup"></a>

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

### Simplest autograded example <a name="simplest"></a>

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

### Checking for loop off-by-one errors and adding more corrections <a name="offbyone"></a>

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

### Generalized format, arbitrary list sizes, more errors <a name="arbitrary"></a>

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

### Validating the author's results on computeDeriv submission A <a name="validating"></a>

For the next step of my analysis, I wanted to apply the author's full and complete process to one of their examples. I chose to use the student submitted ``computeDeriv`` code from Figure 2 (a), since it was straightforward to implement in C, and explained further in Figure 4.

My reference soultion for ``computeDeriv`` looked like this. Without dynamically-sized lists in Sketch, I had to use and update the variable ``output_len``. This didn't turn out to be an issue:
```
// correct version
int[n] reference_solution(int n, int[n] x, ref int output_len){
   int[n] output;
   
   // if the number is a constant, return [0]
   if (n == 1){
      output[0] = 0;
      output_len = 1;
      return output;
   }
   
   // otherwise, start at the 2nd term and build the result array
   for (int i = 1; i < n; i++){
      output[i - 1] = x[i] * i;
   }

   output_len = n - 1;
   return output;
}
```

The student solution (translated into C++) looked like this:
```
// student version
int[n] student_solution(int n, int[n] x, ref int output_len){
   int[n] output;
   output_len = 0;
   int zero = 0;
   int appendIndex = 0;

   if (n == 1){
      return output;
   }

   for (int i = 0; i < n; i++){
      if (x[i] == 0){
	     zero++;
	  }
	  else{
	     output[appendIndex++] = x[i] * i;
	     output_len++;
	  }
   }

   return output;
}
```

After manually creating and substituting in the three possible errors described at the middle-bottom-left of Page 17:
```
// student version
int[n] student_solution(int n, int[n] x, ref int output_len){
   int[n] output;
   output_len = 0;
   int zero = 0;
   int appendIndex = 0;

   if (condition1(n == 1)){
      return returnValue1(n, output, output_len);
   }

   for (int i = rangeLowerValue(0); i < n; i++){
      if (condition2(x[i] == 0)){
	     zero++;
	  }
	  else{
	     output[appendIndex++] = x[i] * i;
	     output_len++;
	  }
   }

   return returnValue2(n, output, output_len);
}

//possible errors
bit condition1(bit x){
   if (??) return x;
   
   return false;
}

bit condition2(bit x){
   if (??) return x;
   
   return false;
}

int rangeLowerValue(int x){
   if (??) return x;
   
   return x + 1;
}

int[n] returnValue1(int n, int[n] x, ref int return_length){
   if (??) return x;
   
   x[0] = 0;
   return_length = 1;
   return x;
}

int[n] returnValue2(int n, int[n] x, ref int return_length){
   if (??) return x;
   
   x[0] = 0;
   return_length = 1;
   return x;
}
```

The Sketch solver was able to correctly identify each version of the variables, in about 2 seconds of runtime:
```
condition1: x
returnValue1: [0]
rangeLowerValue: x + 1
condition2: false
returnValue2: x
```

## Conclusion <a name="conclusion"></a>

I really enjoyed working with this problem, paper, and framework. There's definitely limitations to it (notably the inability to insert code that is missing; the framework only seems to be able to modify values in existing code), but the results it produces are undeniably correct, and could definitely be helpful in some contexts. Even just the ability to check for common error cases (such as off-by-one errors) is interesting.

## Future work <a name="future"></a>

Many parts of this problem are still on the table, and with more time I would have looked into:
- Performance on more complicated examples
- Multi-type compatible programs
- Automating the parsing + rewriting of programs (pretty large task)
- Generating feedback from the Sketch solver changes
