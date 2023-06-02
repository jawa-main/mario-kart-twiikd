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

int SOSetSocketBlocking(s32 s, bool blocking)
{
    return SOFcntl(s, F_SETFL, blocking ? 0 : O_NONBLOCK);
};

#define EAGAIN_DELAY_MS 1

#define HTTP_11_REQUEST_TEMPLATE "GET %s HTTP/1.1\r\n"        \
                                 "Host: %s\r\n"               \
                                 "Connection: keep-alive\r\n" \
                                 "User-Agent: RVL SDK/1.0"

// recommended size for dst is 0x100+
void http_req_fmt(char *dst, char *path, char *hostname)
{
    sprintf(dst, HTTP_11_REQUEST_TEMPLATE, path, hostname);
}

int util_download_file_http(const char *url, u32 buflen, char* outbuf, NHTTPReqCallback callback)
{
    NHTTPCreateRequest(url, NHTTP_REQMETHOD_GET, outbuf, buflen, callback, outbuf);
};

#endif