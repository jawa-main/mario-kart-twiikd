int OSDisableInterrupts(void);
int OSEnableInterrupts(void);
int OSRestoreInterrupts(int level);
int OSIsEnableInterrupts(void);

void DWC_InitGHTTP(int param);
void DWC_GetGHTTPDataEx(char *url, u32 recv_size, int r5, int r6, void (*cb)(char *buf, int buflen, int result, void *r6), int r8);
void DWC_ProcessGHTTP();