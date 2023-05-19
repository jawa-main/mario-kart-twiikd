#include <kamek.h>
#include <mod/events.h>

#define __sym__GXDrawHook 0x8016eab4
#define __sym__RKSystem_main 0x80008ec8

#define __asm__nop 0x60000000

kmCall(__sym__GXDrawHook, &nkw2_update);
kmCall(__sym__RKSystem_main, &nkw2_start);

kmWrite32(0x80006320, __asm__nop); // preserve code