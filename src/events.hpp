#include <revokart/rk_stdlib.hpp>
#include <revokart/Page.hpp>

#include <twiiks/esrb/esrb_hook.hpp>
#include <twiiks/dvd/dvd_hooks.hpp>
#include <twiiks/szs_getfile/no_invisible_walls.hpp>
#include <twiiks/nand2sd/redirect_nand.hpp>
#include <twiiks/globals.hpp>
#include <util.hpp>

void twiikd_update()
{
    OSRestoreInterrupts(TRUE);
};

#define __dolsym__DVDReadPrio_before_restgpr 0x8015e8a8
#define __dolsym__NANDOpen 0x8019c798
#define __VERSION__ "v1.0.0 alpha"

#define NOP 0x60000000

#define write32(addr, x) *(u32*)addr = x;

void twiikd_start()
{
    // code
    twiikd_printf("Version %s compiled at: %s, %s", __VERSION__, __DATE__, __TIME__);

    EGG_Heap_initialize();
    twiikd_init_globals();

    SDA_FSTRootEntry = (FSTFileEntry *)(*(u32 *)(SDA_Address - 0x65C0));
    SDA_FSTEntryCount = SDA_FSTRootEntry->filesize; // RootEntry is a dir entry, so filesize = entrycount
    SDA_FSTFilenameTableStart = (char *)(SDA_FSTRootEntry + (SDA_FSTEntryCount * 0xC));

    write32(0x800cd030, NOP);
    write32(0x801c09a4, NOP);
};

#define __relsym__System_DVDArchive_getFile 0x80515084

int twiiked_before_rel_load()
{

    __asm("lis r4, 0x8000");
}

int twiikd_after_rel_load()
{
    runtime_branch(0x8064c364, &ESRBInitHook, BRANCHTYPE_BRANCH);

    // shutdown, cleanup, finish

    return 0;
};