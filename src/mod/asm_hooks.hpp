#include <revokart/Page.hpp>

void __asm__Page_initChildren_hook()
{
    // r4 = child count

    // r5 = page id

    asm(
        "lwz r5, 4 (r3)\n"
        "cmpwi r5, 0x6B\n" // character selection

        "beq char_sel\n"
        "b next\n"

        "char_sel:\n"
        "addi r4, r4, 1\n"
        "b end\n"

        "next:\n"

        "end:\n"
        "mr r5, r4\n"
    );
}