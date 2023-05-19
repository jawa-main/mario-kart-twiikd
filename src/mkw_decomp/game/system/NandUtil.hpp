#pragma once

#include <rk_types.h>
#include <rvl/nand/nand.h>
#include <rvl/os/os.h>
#include <rvl/os/osThread.h>
#include <decomp.h>

#ifdef __cplusplus
extern "C" {
#endif

// PAL: 0x8052ae5c..0x8052af64
int NandUtil_safeOpen(const char* a1, NANDFileInfo* a2, u8 a3, void* a4,
                      u32 a5);
int NandUtil_open(const char* a1, NANDFileInfo* a2, u8 a3);
int NandUtil_safeClose(NANDFileInfo* a1);
int NandUtil_close(NANDFileInfo* a1);
// PAL: 0x8052b228..0x8052b3f8
UNKNOWN_FUNCTION(NandUtil_read);
// PAL: 0x8052b3f8..0x8052b4d4
UNKNOWN_FUNCTION(NandUtil_getLength);
// PAL: 0x8052b4d4..0x8052b684
UNKNOWN_FUNCTION(NandUtil_write);
// PAL: 0x8052b684..0x8052b754
UNKNOWN_FUNCTION(NandUtil_check);
// PAL: 0x8052b754..0x8052b84c
UNKNOWN_FUNCTION(NandUtil_createDir);
// PAL: 0x8052b84c..0x8052b95c
UNKNOWN_FUNCTION(NandUtil_getType);
// PAL: 0x8052b95c..0x8052ba54
UNKNOWN_FUNCTION(NandUtil_create);
// PAL: 0x8052ba54..0x8052bb40
UNKNOWN_FUNCTION(NandUtil_delete);
// PAL: 0x8052bb40..0x8052bc3c
UNKNOWN_FUNCTION(NandUtil_setStatus);
// PAL: 0x8052bc3c..0x8052bd38
int NandUtil_getStatus(void*, void*);
void unk_8052bd38(void* first);

#ifdef __cplusplus
}
#endif
