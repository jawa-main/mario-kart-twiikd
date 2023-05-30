#include <revokart/rk_stdlib.hpp>
#include <twiiks/globals.hpp>

u32 DVDReadPrio_hook(u32 readlen)
{
    DVDFileInfo* info;
    void* buf;

    __asm("mr %0, r27" : "=r"(info));
    __asm("mr %0, r28" : "=r"(buf));

    twiikd_printf("DVDReadPrio file %s, len %x, buf %x", DVDConvertFileInfoToPath(info), readlen, buf);

    return readlen;
};