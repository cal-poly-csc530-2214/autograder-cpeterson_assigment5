#ifndef TEST4_H
#define TEST4_H

#include <cstring>

#include "vops.h"

namespace ANONYMOUS{
}
namespace ANONYMOUS{
extern void arbitrary_length__Wrapper(int n, int* x/* len = n */);
extern void arbitrary_length__WrapperNospec(int n, int* x/* len = n */);
extern void arbitrary_length(int n, int* x/* len = n */);
extern void student_solution(int n, int* x/* len = n */, int* _out/* len = n */);
extern void reference_solution(int n, int* x/* len = n */, int* _out/* len = n */);
extern void offByOne(int x, int& _out);
extern void offByOne2(int x, int& _out);
extern void correctOperator(int x, int y, int& _out);
}

#endif
