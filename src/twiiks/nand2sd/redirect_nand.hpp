#include <revokart/rk_stdlib.hpp>

void NANDOpen_hook()
{
    char* dst;
    __asm("addi %0, r30, 8" : "=r"(dst));

    // nadn
};