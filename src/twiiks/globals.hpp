#ifndef RK_GLOBALS
#define RK_GLOBALS

#include <revokart/rk_stdlib.hpp>

static hashmap* g_DVD_path_entrynum_dict;

void twiikd_init_globals()
{
    g_DVD_path_entrynum_dict = &hashmap_init(HASHMAP_MAX_ENTRY_COUNT);
};

#endif