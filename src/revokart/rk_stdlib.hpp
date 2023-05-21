#include <revokart/rk_types.hpp>
#include <revokart/rvl_sdk_lite.hpp>

struct hashmap_entry
{
    
};

struct hashmap
{
    /* hash: compute hash value of string */
    unsigned int hash(char *str)
    {
        unsigned int h;
        unsigned char *p;

        h = 0;
        for (p = (unsigned char *)str; *p != '\0'; p++)
            h = 31 * h + *p;
        return h; // or, h % ARRAY_SIZE;
    }
};