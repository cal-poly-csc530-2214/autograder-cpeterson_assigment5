#ifndef TEST3_H
#define TEST3_H

#include <cstring>

#include "vops.h"

namespace ANONYMOUS{
}
namespace ANONYMOUS{
extern void add_one_to_each__Wrapper(int* x/* len = 5 */);
extern void add_one_to_each__WrapperNospec(int* x/* len = 5 */);
extern void add_one_to_each(int* x/* len = 5 */);
extern void offByOne(int x, int& _out);
extern void offByOne2(int x, int& _out);
}

#endif
