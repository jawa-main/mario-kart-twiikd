#include <rvl_sdk_lite.hpp>
#include <mod/rel_hooks.hpp>
#include <revokart/Page.hpp>
#include <mod/asm_hooks.hpp>
#include <util.hpp>

#define BL_MASK 0b01001000000000000000000000000000
#define DELTA_MASK 0b00000011111111111111111111111100

void hook_rel(u32 address, void *func)
{
    u32 delta = (u32)func - address;
    u32 bl = BL_MASK | (delta & DELTA_MASK) | 1;
    *(u32 *)address = bl;

    // OSReport("nkw: bl valid: %x == %x, %s\n", bl, 0x4BCB69D5, bl == 0x4BCB69D5 ? "true" : "false");
    address |= 1;
};

void twiikd_update()
{
    // OSReport("nkw update: %x\n", &twiikd_update);

    OSRestoreInterrupts(TRUE);
};

void twiikd_start()
{
    asm("stw r3, -0x6C18 (r13)");

    // code
    OSReport("nkw start: %x\n", &twiikd_start);
};

#define __relsym__Page_initChildren 0x805ddb7c

int twiikd_after_rel_load()
{
    // hook rel functions
    // hook_rel(0x806097d4, &TitlePage_onInit_hook);

    OSReport("twiikd after rel load: %x\n", &twiikd_after_rel_load);

    // OSReport("Function addresses:\n");
    // OSReport("  Page_initChildren_hook: %x\n", &__asm__Page_initChildren_hook);

    // hook_rel(__relsym__Page_initChildren, &__asm__Page_initChildren_hook);

    return 0;
};