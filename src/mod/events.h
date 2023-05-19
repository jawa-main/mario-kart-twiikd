#include <rvl_sdk.h>
#include <mod/rel_hooks.h>

void hook_rel(u32 address, void *func)
{
    OSReport("Hooking %llx -> %llx", address, func);
    *(u32 *)address = 0x49000000 - (address - (u32)func);
    address |= 1;
}

void nkw2_update()
{
    OSRestoreInterrupts(1);
};

void nkw2_start()
{
    OSReport("nkw2_start\n");
    asm("stw r3, -0x6C18 (r13)");

    // hook rel functions

    hook_rel(0x806097d4, TitlePage_onInit_hook);
};