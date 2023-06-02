#ifndef __RK_FUNC
#define __RK_FUNC

#include <revokart/rk_types.hpp>

#define twiikd_printf(msg, ...) OSReport("%s:%d: " msg "\n", __FILE__, __LINE__, __VA_ARGS__)
#define malloc(sz) EGG_Heap_alloc(sz, 0x20, NULL)

void _restgpr_27();
void _restgpr_24();

void _savegpr_24();

void EGG_Heap_initialize();
void *EGG_Heap_alloc(u32 size, u32 alignment, void *egg_heap_ptr);
void* free(void* block);

void nandGenerateAbsPath(char* dst, char* path);

void* SOLibraryConfig_SOAlloc(u32 name, s32 size);
void SOLibraryConfig_SOFree(u32 name, void *ptr, s32 size);

void* RKNetController_initStaticInstance(void* egg_heap);
void NetManager_connect(void* rknet);

void DirectPrint_DrawString(u32 x, u32 y, bool wrap_text, char* fmt, ...);
void DirectPrint_SetupFB(void* fb);
typedef enum
{
    NHTTP_REQMETHOD_GET,
    NHTTP_REQMETHOD_POST,
    NHTTP_REQMETHOD_HEAD,
    NHTTP_REQMETHOD_MAX
} NHTTPReqMethod;
struct NHTTPResponseHandle
{
    volatile int headerLen;    // �w�b�_(�擪����\r\n\r\n�܂�)�̒���[byte]
    volatile int bodyLen;      // �{�f�B�̒���[byte]
    volatile int totalBodyLen; // �{�f�B�̒���[byte]
    volatile int contentLength;
    int isSuccess;
    int isHeaderParse;
    int httpStatus;           // httpStatus
    unsigned long recvBufLen; // recvBuf_p�̑S��
    char *allHeader_p;
    char *foundHeader_p;
    char *recvBuf_p;      // �G���e�B�e�B�{�f�B��M�o�b�t�@
    void* bufFull; // ��M�o�b�t�@���R�[���o�b�N�֐�
    void* freeBuf; // ��M�o�b�t�@����R�[���o�b�N�֐�
    void *hdrBufBlock_p;
    char hdrBufFirst[0x400];
    void *param_p; // createRequest�Őݒ肵��param
};
typedef void (*NHTTPReqCallback)(int error, NHTTPResponseHandle* res, void *param);
void NHTTP_CreateRequest(const char *url, NHTTPReqMethod method, char *buf, u32 len, NHTTPReqCallback callback, void *param);

#endif