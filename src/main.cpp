#include <kamek.h>
#include <mod/events.hpp>

#define __sym__GXDrawHook 0x8016eab4
#define __sym__RKSystem_main 0x80008ec8
#define __sym__ModuleLinker_linkModule 0x8000a3a0

#define __asm__nop 0x60000000

kmCall(__sym__GXDrawHook, &twiikd_update);
kmCall(__sym__RKSystem_main, &twiikd_start);
kmCall(__sym__ModuleLinker_linkModule, &twiikd_after_rel_load);

kmWrite32(0x80006320, __asm__nop); // preserve code