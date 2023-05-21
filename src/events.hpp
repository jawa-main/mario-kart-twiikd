#include <revokart/rk_stdlib.hpp>
#include <revokart/Page.hpp>
#include <twiiks/szs_getfile/no_invisible_walls.hpp>
#include <util.hpp>

#define BL_MASK 0b01001000000000000000000000000000
#define DELTA_MASK 0b00000011111111111111111111111100

void rel_branch(u32 address, void *func, bool link = true)
{
    u32 delta = (u32)func - address;
    u32 bl = BL_MASK | (delta & DELTA_MASK);
    bl |= link;
    *(u32 *)address = bl;
};

void twiikd_update()
{
    // OSReport("nkw update: %x\n", &twiikd_update);

    OSRestoreInterrupts(TRUE);
};

void twiikd_start()
{
    // code
    OSReport("nkw start: %x\n", &twiikd_start);

    __asm("lis r4, 0x8000");
};

// #define __relsym__Page_initChildren 0x805ddb7c
#define __relsym__System_DVDArchive_getFile 0x80515084

int twiikd_after_rel_load()
{
    // hook rel functions
    // hook_rel(0x806097d4, &TitlePage_onInit_hook);

    OSReport("twiikd after rel load: %x\n", &twiikd_after_rel_load);

    OSReport("Function addresses:\n");
    OSReport("  System_DVDArchive_getFile_hook: %x\n", &System_DVDArchive_getFile_hook);

    // rel_branch(__relsym__System_DVDArchive_getFile, &System_DVDArchive_getFile_hook);
    // *(u32 *)(__relsym__System_DVDArchive_getFile + 4) = 0x7D8803A6; // mtlr r12

    return 0;
};

void* DVDReadPrio_hook(s32 read_len)
{
    void* buf;
    DVDFileInfo* dvd_file_info;

    __asm("mr %0, r28" : "=r" (buf));
    __asm("mr %0, r27" : "=r" (dvd_file_info));

    

    OSReport("DVDReadPrio entry %d filename %s\n", )
    return buf;
};