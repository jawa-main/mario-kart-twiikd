#include <kamek.h>
#include <events.hpp>
#include <util.hpp>

#define __sym__GXDrawHook 0x8016eab4
#define __sym__start 0x800061c8
#define __sym__RKSystem_initialize_end 0x800094d8
#define __sym__ModuleLinker_linkModule 0x8000a3a0
#define __sym__DVDReadPrio 0x8015e8a8

#define __asm__nop 0x60000000

#define NOWARN #pragma warning(disable:1097)

kmCall(__sym__GXDrawHook, &twiikd_update);
kmCall(__sym__start, &twiiked_before_rel_load);
kmCall(__sym__ModuleLinker_linkModule, &twiikd_after_rel_load);

kmBranch(__sym__RKSystem_initialize_end, &twiikd_start);

kmWrite32(&OSReport, 0x4E800020); // insert blr at the beginning of OSReport
kmWrite32(0x80006320, __asm__nop); // preserve code