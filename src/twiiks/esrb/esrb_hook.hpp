#include <util.hpp>

void *ESRBInitHook(void *r3)
{
    u8 *outbuf = (u8 *)malloc(0x1000);
    util_download_file_http("info.cern.ch/hypertext/WWW/TheProject.html", outbuf, 0x1000);
    twiikd_printf("%x", outbuf);
    return r3;
};