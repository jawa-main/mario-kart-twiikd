#ifndef RK_UTIL
#define RK_UTIL

#include <revokart/rk_stdlib.hpp>

#define PAD_CCP1_ADDR 0x14C2
#define PAD_INPUT_CCP_MINUS 0x1000

u16 util_get_controller_inputs(u16 controller_offset)
{
    return *(u16 *)(0x80340000 + controller_offset + 2);
};

#define BL_MASK 0b01001000000000000000000000000000
#define DELTA_MASK 0b00000011111111111111111111111100

#define BRANCHTYPE_BRANCH 0
#define BRANCHTYPE_LINK 1

void runtime_branch(u32 address, void *func, u8 branch_type)
{
    u32 delta = (u32)func - address;
    u32 bl = BL_MASK | (delta & DELTA_MASK);
    bl |= branch_type;
    *(u32 *)address = bl;
};

bool g_SOInitialized = false;

int util_init_socket()
{
    RKNetController_initStaticInstance(NULL);

    SOLibraryConfig cfg = {SOLibraryConfig_SOAlloc, SOLibraryConfig_SOFree};

    twiikd_printf("SOInit: %d", SOInit(&cfg));
}

int SOSetSocketBlocking(s32 s, bool blocking)
{
    return SOFcntl(s, F_SETFL, blocking ? 0 : O_NONBLOCK);
};

// url MOST NOT start with http://
// call after rel load
int util_download_file_http(char *url, u8 *outbuf, u32 buflen, bool debug = true)
{
    if (!g_SOInitialized)
    {
        twiikd_printf("%s", "SO not initialized");
        util_init_socket();
    }

    int urlen = strlen(url);
    char hostname[64];

    char *url_path = strchr(url, '/');
    strncpy(hostname, url, (u32)(url_path - url));

    if (debug)
    {
        twiikd_printf("Hostname: %s", hostname);
        twiikd_printf("Url path: %s", url_path);
    }

    int so_startup = SOStartup();
    if (debug)
        twiikd_printf("SOStartup: %d", so_startup);

    int s = SOSocket2(2, 1, 0);
    if (debug)
        twiikd_printf("socket: %d", s);

    twiikd_printf("fcntl blocking: %d", SOSetSocketBlocking(s, false));

    hostent *he = SOGetHostByName(hostname);

    u32 sin_addr = *(u32 *)he->h_addr_list[0];

    if (debug)
        twiikd_printf("h_len: %d", he->h_length);
    if (debug)
        twiikd_printf("*h_addr_list[0]: %x", sin_addr);

    sockaddr_in saddr;
    if (debug)
        twiikd_printf("ip of %s: %d.%d.%d.%d", hostname, (sin_addr & 0xFF000000) >> 24, (sin_addr & 0xFF0000) >> 16, (sin_addr & 0xFF00) >> 8, sin_addr & 0xFF);

    saddr.sin_addr.s_addr = sin_addr;
    saddr.sin_family = 2;
    saddr.sin_port = 80;
    saddr.sin_len = 8;

    int conn = SOConnect(s, (sockaddr *)&saddr, sizeof(saddr));
    if (debug)
        twiikd_printf("SOConnect: %d", conn);

    char req[512];
    sprintf(req, "GET %s HTTP/1.1\r\nHost: %s", url_path, hostname);
    if (debug)
        twiikd_printf("Sending request: %s", req);

    int sosend = SOSend(s, req, strlen(req), 0);
    if (debug)
        twiikd_printf("SOSend: %d", sosend);

    int sorecv = SO_EAGAIN;
    while (sorecv == SO_EAGAIN)
    {
        sorecv = SORecv(s, outbuf, buflen, 4);
        if (debug)
            twiikd_printf("SORecv: %d", sorecv);
        msleep(1000);
    }
    if (debug)
        twiikd_printf("%x", outbuf);

    SOClose(s);
};

#endif