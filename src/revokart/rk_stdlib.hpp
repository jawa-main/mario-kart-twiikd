#ifndef RK_STDLIB
#define RK_STDLIB

#include <revokart/rk_types.hpp>
#include <revokart/rvl_sdk_lite.hpp>
#include <revokart/rk_func.hpp>

#pragma region hashmap

#define HASHMAP_MAX_ENTRY_COUNT 4096 // (4096 * 8) + 4 bytes = ~32 KB
#define HASHMAP_MAX_KB_SIZE ((HASHMAP_MAX_ENTRY_COUNT * 8) + 4) / 1024

#define HASHMAP_ENOMEM -1
#define HASHMAP_ENOKEY -2
#define HASHMAP_ENOKEYIDX -3

#define SDA_Address 0x80388880

#define FST_ENTRYTYPE_FILE 0 << 24
#define FST_ENTRYTYPE_DIR 1 << 24

struct FSTFileEntry
{
    u32 entrytype_and_filename_offset;
    u32 disk_offset;
    u32 filesize;
};

static FSTFileEntry *SDA_FSTRootEntry;
static char *SDA_FSTFilenameTableStart;
static u32 SDA_FSTEntryCount;

s32 DVDConvertFileInfoToEntrynum(DVDFileInfo *file_info)
{
    u32 disk_offset = file_info->startAddr;

    for (s32 i = 0; i < SDA_FSTEntryCount; i++)
    {
        FSTFileEntry *entry = SDA_FSTRootEntry + (i * 0xC);

        if (entry->disk_offset == disk_offset)
        {
            return i;
        }
    }

    twiikd_printf("Could not find file at offset: %x\n", disk_offset);

    return -1;
};

char *DVDConvertEntrynumToPath(s32 entrynum)
{
    FSTFileEntry *entry = (FSTFileEntry *)(SDA_Address + (entrynum * sizeof(FSTFileEntry)));
    char *path = SDA_FSTFilenameTableStart + (entry->entrytype_and_filename_offset & 0xFFFFFF);

    twiikd_printf("Path of entrynum %d is '%s'", entrynum, path);

    return path;
};

char *DVDConvertFileInfoToPath(DVDFileInfo *info)
{
    return DVDConvertEntrynumToPath(DVDConvertFileInfoToEntrynum(info));
};

/* hash: compute hash value of string */
u32 hash_string(char *str)
{
    u32 h;
    u8 *p;

    h = 0;
    for (p = (u8 *)str; *p != '\0'; p++)
        h = 31 * h + *p;
    return h; // or, h % ARRAY_SIZE;
};

typedef struct hashmap_entry
{
    u32 key_hash;
    void *value_ptr;
} hashmap_entry;

typedef struct hashmap
{
    u32 kv_pairs_len;
    hashmap_entry *kv_pairs;
} hashmap;

hashmap hashmap_init(u32 maxlen)
{
    hashmap ret_map = {};

    ret_map.kv_pairs_len = 0;
    ret_map.kv_pairs = (hashmap_entry *)malloc(maxlen * sizeof(hashmap_entry *));

    return ret_map;
}

void *hashmap_get(hashmap *map, char *key)
{
    u32 hash = hash_string(key);

    for (u32 i = 0; i < map->kv_pairs_len; i++)
    {
        hashmap_entry entry = map->kv_pairs[i];

        if (entry.key_hash == hash)
        {
            return entry.value_ptr;
        }
    }

    twiikd_printf("TWIIKD_HASHMAP | ERROR | Key %s not found", key);

    return NULL;
};

u32 hashmap_index(hashmap *map, u32 key_hash)
{
    for (size_t i = 0; i < map->kv_pairs_len; i++)
    {
        twiikd_printf("hashmap_index: %x == %x", key_hash, map->kv_pairs[i].key_hash);
        if (key_hash == map->kv_pairs[i].key_hash)
        {
            return i;
        }
    }

    twiikd_printf("TWIIKD_HASHMAP | ERROR | Index of key hash %x not found", key_hash);

    return HASHMAP_ENOKEYIDX;
}

void hashmap_put(hashmap *map, char *key, void *value)
{
    u32 key_hash = hash_string(key);

    u32 index = hashmap_index(map, key_hash);

    if (index == HASHMAP_ENOKEYIDX)
    {
        index = map->kv_pairs_len;

        hashmap_entry entry;

        entry.key_hash = key_hash;
        entry.value_ptr = value;

        map->kv_pairs[index] = entry;
        map->kv_pairs_len++;
    }

    hashmap_entry *entry_ptr = &map->kv_pairs[index];

    entry_ptr->key_hash = key_hash;
    entry_ptr->value_ptr = value;
}

#pragma endregion
#pragma region strings

u32 strcount(char *str, char c)
{
    u32 count = 0;
    while (*str)
        if (*str++ == c)
            ++count;
    return count;
};

void strreplace(char *str, char find, char replace)
{
    char *current_pos = strchr(str, find);
    while (current_pos)
    {
        *current_pos = replace;
        current_pos = strchr(current_pos, find);
    }
}
#pragma endregion
#endif