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
    void* value_ptr;
} hashmap_entry;

typedef struct hashmap
{
    u32 kv_pairs_len;
    hashmap_entry* kv_pairs;
} hashmap;

hashmap hashmap_init(u32 maxlen)
{
    hashmap ret_map = {};

    ret_map.kv_pairs_len = 0;
    ret_map.kv_pairs = (hashmap_entry*)malloc(maxlen * sizeof(hashmap_entry*));

    return ret_map;
}

void* hashmap_get(hashmap* map, char* key)
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

    OSReport("TWIIKD_HASHMAP | ERROR | Key %s not found\n", key);

    return NULL;
};

u32 hashmap_index(hashmap* map, char* key)
{
    for (size_t i = 0; i < map->kv_pairs_len; i++)
    {
        if (hash_string(key) == map->kv_pairs[i].key_hash)
        {
            return i;
        }
    }

    OSReport("TWIIKD_HASHMAP | ERROR | Index of key %s not found\n", key);

    return HASHMAP_ENOKEYIDX;
}

void hashmap_put(hashmap* map, char* _key, void* value)
{
    char* key;

    strcpy(key, _key);

    u32 index = hashmap_index(map, key);

    if (index == HASHMAP_ENOKEYIDX)
    {
        OSReport("TWIIKD_HASHMAP | ERROR | Aborting hashmap_put with error code HASHMAP_ENOKEYIDX\n");

        return;
    }

    hashmap_entry* entry_ptr = &map->kv_pairs[index];

    entry_ptr->key_hash = hash_string(key);
    entry_ptr->value_ptr = value;
}

#pragma endregion

#endif