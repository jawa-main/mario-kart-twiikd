#include <util.hpp>

#define URL "info.cern.ch/hypertext/WWW/TheProject.html"
void nhttp_cb(int error, NHTTPResponseHandle* res, void *param)
{
    twiikd_printf("downloaded buf: %x\n", param);
};
void *ESRBInitHook(void *r3)
{
    char* buf = (char*) malloc(0x1000);
    util_download_file_http(URL, 0x1000, buf, nhttp_cb);
    return r3;
};