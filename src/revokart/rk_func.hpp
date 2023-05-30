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

void DirectPrint_DrawString(u32 x, u32 y, bool wrap_text, char* fmt, ...);
void DirectPrint_SetupFB(void* fb);

#endif