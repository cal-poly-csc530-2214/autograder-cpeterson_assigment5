#ifndef TEST5_H
#define TEST5_H

#include <cstring>

#include "vops.h"

namespace ANONYMOUS{
}
namespace ANONYMOUS{
extern void arbitrary_length__Wrapper(int n, int* x/* len = n */);
extern void arbitrary_length__WrapperNospec(int n, int* x/* len = n */);
extern void arbitrary_length(int n, int* x/* len = n */);
extern void student_solution(int n, int* x/* len = n */, int& output_len, int* _out/* len = n */);
extern void reference_solution(int n, int* x/* len = n */, int& output_len, int* _out/* len = n */);
extern void condition1(bool x, bool& _out);
extern void returnValue1(int n, int* x_0/* len = n */, int& return_length, int* _out/* len = n */);
extern void rangeLowerValue(int x, int& _out);
extern void condition2(bool x, bool& _out);
extern void returnValue2(int n, int* x_0/* len = n */, int& return_length, int* _out/* len = n */);
}

#endif
