#include <revokart/rk_stdlib.hpp>
#include <twiiks/globals.hpp>

// function end
u32 DVDConvertPathToEntrynum_hook(u32 entrynum)
{
    char* filename;

    __asm("mr %0, r25\n" : "=r"(filename));

    OSReport("Storing KV pair {%s:0x%x} to g_DVD_path_entrynum_dict\n", filename, entrynum);
    hashmap_put(g_DVD_path_entrynum_dict, filename, (void*)entrynum);

    return entrynum;
}