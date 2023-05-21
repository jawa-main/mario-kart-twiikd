#include <revokart/rk_types.hpp>

#define LAP_ADDR_0 (u32*)0x8052eb63
#define LAP_ADDR_1 (u32 *)(((u32) LAP_ADDR_0) + 40)

void set_lap_count(u8 lapCount)
{
    *LAP_ADDR_0 = lapCount;
    *LAP_ADDR_1 = lapCount;
}