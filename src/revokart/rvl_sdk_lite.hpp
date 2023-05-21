#ifndef RVL_SDK_LITE
#define RVL_SDK_LITE

void OSRestoreInterrupts(int level);
void OSReport(const char* fmt,...);

#pragma region ARC

#include <revokart/rk_types.hpp>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        unsigned int magic;
        int fstStart;
        int fstSize;
        int fileStart;
        int reserve[4];

    } ARCHeader;

#define DARCH_MAGIC 0x55aa382d

    typedef struct
    {
        void *archiveStartAddr;
        void *FSTStart;
        void *fileStart;
        u32 entryNum;
        char *FSTStringStart;
        u32 FSTLength;
        u32 currDir;

    } ARCHandle;

    typedef struct
    {
        ARCHandle *handle;
        u32 startOffset;
        u32 length;

    } ARCFileInfo;

    typedef struct
    {
        ARCHandle *handle;
        u32 entryNum;
        u32 location;
        u32 next;
    } ARCDir;

    typedef struct
    {
        ARCHandle *handle;
        u32 entryNum;
        bool isDir;
        char *name;
    } ARCDirEntry;

    bool ARCInitHandle(void *arcStart, ARCHandle *handle);
    bool ARCOpen(ARCHandle *handle, const char *fileName, ARCFileInfo *af);
    bool ARCFastOpen(ARCHandle *handle, s32 entrynum, ARCFileInfo *af);
    s32 ARCConvertPathToEntrynum(ARCHandle *handle, const char *pathPtr);
    bool ARCEntrynumIsDir(const ARCHandle *handle, s32 entrynum);
    void *ARCGetStartAddrInMem(ARCFileInfo *af);
    u32 ARCGetStartOffset(ARCFileInfo *af);
    u32 ARCGetLength(ARCFileInfo *af);
    bool ARCClose(ARCFileInfo *af);

    bool ARCChangeDir(ARCHandle *handle, const char *dirName);
    bool ARCGetCurrentDir(ARCHandle *handle, char *path, u32 maxlen);

    bool ARCOpenDir(ARCHandle *handle, const char *dirName, ARCDir *dir);
    bool ARCReadDir(ARCDir *dir, ARCDirEntry *dirent);
    bool ARCCloseDir(ARCDir *dir);

/*---------------------------------------------------------------------------*
  Name:         ARCTellDir

  Description:  Returns the current location associated with the directory

  Arguments:    dir         Pre-opened ARCDir* structure

  Returns:      current location
 *---------------------------------------------------------------------------*/
#define ARCTellDir(dir) ((dir)->location)

/*---------------------------------------------------------------------------*
  Name:         ARCSeekDir

  Description:  Sets the position of the next ARCReadDir on the directory

  Arguments:    dir         Pre-opened ARCDir* structure
                loc         location to set

  Returns:      None
 *---------------------------------------------------------------------------*/
#define ARCSeekDir(dir, loc) ((dir)->location = loc)

/*---------------------------------------------------------------------------*
  Name:         ARCRewindDir

  Description:  Resets the position of the directory to the beginning

  Arguments:    dir         Pre-opened ARCDir* structure

  Returns:      None
 *---------------------------------------------------------------------------*/
#define ARCRewindDir(dir) ((dir)->location = (dir)->entryNum + 1)

#define ARCGetDirEntryName(dirent) ((dirent)->name)
#define ARCDirEntryIsDir(dirent) ((dirent)->isDir)

#pragma endregion
#pragma region DVD
#ifdef __cplusplus
}
#endif

#ifndef __DVD_H__
#define __DVD_H__

#include <revokart/rk_types.hpp>

#ifdef __cplusplus
extern "C"
{
#endif
// the following is a restriction that only exists on Mac emulator
#define DVD_PATH_MAX 200 // The number of bytes in an absolute pathname
                         // excluding terminating '\0'.

    typedef struct DVDDiskID DVDDiskID;

    struct DVDDiskID
    {
        char gameName[4];
        char company[2];
        u8 diskNumber;
        u8 gameVersion;
        u8 streaming;
        u8 streamingBufSize; // 0 = default
        u8 padding[14];      // 0's are stored
        u32 rvlMagic;        // Revolution disk magic number
        u32 gcMagic;         // GC magic number is here
    };

    typedef struct DVDCommandBlock DVDCommandBlock;

    typedef void (*DVDCBCallback)(s32 result, DVDCommandBlock *block);

    struct DVDCommandBlock
    {
        DVDCommandBlock *next;
        DVDCommandBlock *prev;
        u32 command;
        s32 state;
        u32 offset;
        u32 length;
        void *addr;
        u32 currTransferSize;
        u32 transferredSize;
        DVDDiskID *id;
        DVDCBCallback callback;
        void *userData;
    }; // size 48


    typedef struct DVDFileInfo DVDFileInfo;

    /*---------------------------------------------------------------------------*
      Name:         DVDCallback

      Description:  DVD callback function

      Arguments:    result      If the function that set the callback succeeds,
                                it is the number of bytes transferred. If that
                                function fails, it is -1.

      Returns:      None.
     *---------------------------------------------------------------------------*/
    typedef void (*DVDCallback)(s32 result, DVDFileInfo *fileInfo);

    struct DVDFileInfo
    {
        DVDCommandBlock cb;

        u32 startAddr; // disk address of file
        u32 length;    // file size in bytes

        DVDCallback callback;
    };

    typedef struct
    {
        u32 entryNum;
        u32 location;
        u32 next;
    } DVDDir;

    typedef struct
    {
        u32 entryNum;
        bool isDir;
        char *name;
    } DVDDirEntry;

// FST maximum size
#ifndef MINNOW
#define DVD_FST_MAX_SIZE 0x00100000
#else // MINNOW
#define DVD_FST_MAX_SIZE 0x00080000
#endif

#define DVD_MIN_TRANSFER_SIZE 32

#define DVD_STATE_FATAL_ERROR -1
#define DVD_STATE_END 0
#define DVD_STATE_BUSY 1
#define DVD_STATE_WAITING 2
#define DVD_STATE_COVER_CLOSED 3 // internal state. never returns.
#define DVD_STATE_NO_DISK 4
#define DVD_STATE_COVER_OPEN 5
#define DVD_STATE_WRONG_DISK 6
#define DVD_STATE_MOTOR_STOPPED 7
#define DVD_STATE_PAUSING 8
#define DVD_STATE_IGNORED 9
#define DVD_STATE_CANCELED 10
#define DVD_STATE_RETRY 11
#define DVD_STATE_NO_INPUT 12

#define DVD_FILEINFO_READY 0
#define DVD_FILEINFO_BUSY 1

#define DVD_RESULT_GOOD 0
#define DVD_RESULT_FATAL_ERROR -1
#define DVD_RESULT_IGNORED -2
#define DVD_RESULT_CANCELED -3

#define DVD_AIS_SUCCESS 0x0

#define DVDGetStartAddr(fileInfo) \
    ((u64)((u64)(fileInfo)->startAddr) << 2)

#define DVDGetLength(fileInfo) \
    ((fileInfo)->length)

    void DVDInit(void);

    bool DVDOpen(const char *fileName, DVDFileInfo *fileInfo);
    bool DVDFastOpen(s32 entrynum, DVDFileInfo *fileInfo);

#ifndef MACOS
    s32 DVDReadPrio(DVDFileInfo *fileInfo, void *addr, s32 length,
                    s32 offset, s32 prio);

    bool DVDReadAsyncPrio(DVDFileInfo *fileInfo, void *addr, s32 length,
                          s32 offset,
                          DVDCallback callback, s32 prio);

#define DVDRead(fileInfo, addr, length, offset) \
    DVDReadPrio((fileInfo), (addr), (length), (offset), 2)
#define DVDReadAsync(fileInfo, addr, length, offset, callback) \
    DVDReadAsyncPrio((fileInfo), (addr), (length), (offset), (callback), 2)
#else
    s32 DVDRead(DVDFileInfo *fileInfo, void *addr, s32 length,
                s32 offset);

    bool DVDReadAsync(DVDFileInfo *fileInfo, void *addr, s32 length,
                      s32 offset,
                      DVDCallback callback);
#endif

    bool DVDClose(DVDFileInfo *fileInfo);

#ifndef MACOS
    bool DVDGetCurrentDir(char *path, u32 maxlen);
#endif

    bool DVDChangeDir(const char *dirName);

#ifdef MACOS
    bool DVDMovePickup(DVDFileInfo *fileInfo,
                       s32 offset,
                       DVDCallback callback);
#else // EPPC
    s32 DVDSeekPrio(DVDFileInfo *fileInfo,
                    s32 offset, s32 prio);

    bool DVDSeekAsyncPrio(DVDFileInfo *fileInfo,
                          s32 offset,
                          DVDCallback callback, s32 prio);

#define DVDSeek(fileInfo, offset) \
    DVDSeekPrio((fileInfo), (offset), 2)
#define DVDSeekAsync(fileInfo, offset, callback) \
    DVDSeekAsyncPrio((fileInfo), (offset), (callback), 2)

    s32 DVDChangeDisk(DVDCommandBlock *block, DVDDiskID *id);

    bool DVDChangeDiskAsync(DVDCommandBlock *block, DVDDiskID *id,
                            DVDCBCallback callback);
#endif

    s32 DVDGetCommandBlockStatus(const DVDCommandBlock *block);

    s32 DVDGetFileInfoStatus(const DVDFileInfo *fileInfo);

#define DVDGetFileInfoStatus(fileinfo) \
    DVDGetCommandBlockStatus(&(fileinfo)->cb)

    s32 DVDGetDriveStatus(void);

    bool DVDSetAutoInvalidation(bool autoInval);

    void *DVDGetFSTLocation(void);

#ifdef MACOS
    void DVDSetRoot(const char *rootPath);
#endif
#ifdef WIN32
    void DVDSetRoot(const char *rootPath);
#endif

    s32 DVDConvertPathToEntrynum(const char *pathPtr);

    bool DVDEntrynumIsDir(s32 entrynum);

    // Directory access functions
    bool DVDOpenDir(const char *dirName, DVDDir *dir);
    bool DVDFastOpenDir(s32 entrynum, DVDDir *dir);
    bool DVDReadDir(DVDDir *dir, DVDDirEntry *dirent);
    bool DVDCloseDir(DVDDir *dir);

    void DVDPause(void);
    void DVDResume(void);

/*---------------------------------------------------------------------------*
  Name:         DVDTellDir

  Description:  Returns the current location associated with the directory

  Arguments:    dir         Pre-opened DVDDir* structure

  Returns:      current location
 *---------------------------------------------------------------------------*/
#define DVDTellDir(dir) ((dir)->location)

/*---------------------------------------------------------------------------*
  Name:         DVDSeekDir

  Description:  Sets the position of the next DVDReadDir on the directory

  Arguments:    dir         Pre-opened DVDDir* structure
                loc         location to set

  Returns:      None
 *---------------------------------------------------------------------------*/
#define DVDSeekDir(dir, loc) ((dir)->location = loc)

    /*---------------------------------------------------------------------------*
      Name:         DVDRewindDir

      Description:  Resets the position of the directory to the beginning

      Arguments:    dir         Pre-opened DVDDir* structure

      Returns:      None
     *---------------------------------------------------------------------------*/
    void DVDRewindDir(DVDDir *dir);

#define DVDGetDirEntryName(dirent) ((dirent)->name)
#define DVDDirEntryIsDir(dirent) ((dirent)->isDir)

/*---------------------------------------------------------------------------*
  Name:         DVDSetUserData

  Description:  Set user data in command block

  Arguments:    block       Command block
                data        Data to set

  Returns:      None
 *---------------------------------------------------------------------------*/
#define DVDSetUserData(block, data) ((block)->userData = (data))

/*---------------------------------------------------------------------------*
  Name:         DVDGetUserData

  Description:  Get user data in command block

  Arguments:    block       Command block

  Returns:      data (void * type)
 *---------------------------------------------------------------------------*/
#define DVDGetUserData(block) ((block)->userData)

#ifndef EMU

    bool DVDCancelAsync(DVDCommandBlock *block, DVDCBCallback callback);
    s32 DVDCancel(DVDCommandBlock *block);
    bool DVDCancelAllAsync(DVDCBCallback callback);
    s32 DVDCancelAll(void);

    s32 DVDGetTransferredSize(DVDFileInfo *fileinfo);

    DVDDiskID *DVDGetCurrentDiskID(void);

    void DVDDumpWaitingQueue(void);

    bool DVDCheckDiskAsync(DVDCommandBlock *block, DVDCBCallback callback);

    bool DVDCompareDiskID(const DVDDiskID *id1, const DVDDiskID *id2);

    DVDDiskID *DVDGenerateDiskID(DVDDiskID *id, const char *game, const char *company,
                                 u8 diskNum, u8 version);

    bool DVDSetAutoFatalMessaging(bool enable);

    bool DVDIsDiskIdentified(void);

    bool dvd_entryToPath_0(s32 entrynum, char *path, u32 maxlen);
    s32 DVDGetEntrynum(const DVDFileInfo *fileInfo);
    const char *DVDGetEntryName(s32 entrynum);

#define DVDGetDirEntryEntrynum(dirent) ((s32)((dirent)->entryNum))
#define DVDGetDirEntrynum(dir) ((s32)((dir)->entryNum))

#endif

#ifdef __cplusplus
}
#endif

#endif // __DVD_H__
#pragma endregion

#endif