#include <rvl_sdk.h>

void download_callback(char *buf, int buflen, int result, void *r6);

void TitlePage_onInit_hook(void *page)
{
    OSReport("Title Page initialized\n");

    // DWC Download Test
    DWC_InitGHTTP(NULL);
    DWC_GetGHTTPDataEx("http://jawa-mkw.tk/index.html", 0x1000, 1, NULL, download_callback, 0);
    DWC_ProcessGHTTP();

    asm("li r4, 0"); // default
};

void download_callback(char *buf, int buflen, int result, void *r6)
{
    OSReport("Buffer saved at: %llx", (u32)buf);
};