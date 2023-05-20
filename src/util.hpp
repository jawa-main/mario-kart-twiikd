#include <kamek.h>

#define PAD_CCP1_ADDR 0x14C2

#define PAD_INPUT_CCP_MINUS 0x1000

u16 util_get_controller_inputs(u16 controller_offset)
{
    return *(u32 *)(0x80340000 + controller_offset + 2);
};