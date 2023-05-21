#include <kamek.h>
#include <events.hpp>

#define __sym__GXDrawHook 0x8016eab4
#define __sym__start 0x800061c8
#define __sym__ModuleLinker_linkModule 0x8000a3a0
#define __sym__DVDReadPrio 0x8015e8a8

#define __asm__nop 0x60000000

kmCall(__sym__GXDrawHook, &twiikd_update);
kmCall(__sym__start, &twiikd_start);
kmCall(__sym__ModuleLinker_linkModule, &twiikd_after_rel_load);
kmBranch(__sym__DVDReadPrio, &DVDReadPrio_hook);

kmWrite32(0x80006320, __asm__nop); // preserve code