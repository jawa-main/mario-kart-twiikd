#ifndef RVL_SDK_LITE
#define RVL_SDK_LITE

void OSRestoreInterrupts(int level);

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
    bool DVDGetCurrentDir(char *path, u32 maxlen);

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
#pragma region stdio

typedef struct _iobuf
{
    void *_Placeholder;
} FILE;

#pragma endregion
#pragma region SO
#define SO_FLAG_DEBUG 0x0001       /* turn on debugging info recording */
#define SO_FLAG_ACCEPTCONN 0x0002  /* socket has had listen() */
#define SO_FLAG_REUSEADDR 0x0004   /* allow local address reuse */
#define SO_FLAG_KEEPALIVE 0x0008   /* keep connections alive */
#define SO_FLAG_DONTROUTE 0x0010   /* just use interface addresses */
#define SO_FLAG_BROADCAST 0x0020   /* permit sending of broadcast msgs */
#define SO_FLAG_USELOOPBACK 0x0040 /* bypass hardware when possible */
#define SO_FLAG_LINGER 0x0080      /* linger on close if data present */
#define SO_FLAG_OOBINLINE 0x0100   /* leave received OOB data in line */
#define SO_FLAG_REUSEPORT 0x0200   /* allow local address & port reuse */

#define SO_SUCCESS 0
#define SO_OK SO_SUCCESS
#define SO_EFATAL 0x80000000
#define SO_E2BIG (-1)
#define SO_EACCES (-2)
#define SO_EADDRINUSE (-3) // Address is already in use
#define SO_EADDRNOTAVAIL (-4)
#define SO_EAFNOSUPPORT (-5) // Non-supported address family
#define SO_EAGAIN (-6)       // Posix.1
#define SO_EALREADY (-7)     // Already in progress
#define SO_EBADF (-8)        // Bad socket descriptor
#define SO_EBADMSG (-9)
#define SO_EBUSY (-10) // Busy
#define SO_ECANCELED (-11)
#define SO_ECHILD (-12)
#define SO_ECONNABORTED (-13) // Connection aborted
#define SO_ECONNREFUSED (-14) // Connection refused
#define SO_ECONNRESET (-15)   // Connection reset
#define SO_EDEADLK (-16)
#define SO_EDESTADDRREQ (-17) // Not bound to a local address
#define SO_EDOM (-18)
#define SO_EDQUOT (-19)
#define SO_EEXIST (-20)
#define SO_EFAULT (-21)
#define SO_EFBIG (-22)
#define SO_EHOSTUNREACH (-23)
#define SO_EIDRM (-24)
#define SO_EILSEQ (-25)
#define SO_EINPROGRESS (-26) // Still in progress
#define SO_EINTR (-27)       // Canceled
#define SO_EINVAL (-28)      // Invalid operation
#define SO_EIO (-29)         // I/O error
#define SO_EISCONN (-30)     // Socket is already connected
#define SO_EISDIR (-31)
#define SO_ELOOP (-32)
#define SO_EMFILE (-33) // No more socket descriptors
#define SO_EMLINK (-34)
#define SO_EMSGSIZE (-35) // Too large to be sent
#define SO_EMULTIHOP (-36)
#define SO_ENAMETOOLONG (-37)
#define SO_ENETDOWN (-38)
#define SO_ENETRESET (-39)
#define SO_ENETUNREACH (-40) // Unreachable
#define SO_ENFILE (-41)
#define SO_ENOBUFS (-42) // Insufficient resources
#define SO_ENODATA (-43)
#define SO_ENODEV (-44)
#define SO_ENOENT (-45)
#define SO_ENOEXEC (-46)
#define SO_ENOLCK (-47)
#define SO_ENOLINK (-48) // Network interface is unavailable
#define SO_ENOMEM (-49)  // Insufficient memory
#define SO_ENOMSG (-50)
#define SO_ENOPROTOOPT (-51) // Non-supported option
#define SO_ENOSPC (-52)
#define SO_ENOSR (-53)
#define SO_ENOSTR (-54)
#define SO_ENOSYS (-55)
#define SO_ENOTCONN (-56) // Not connected
#define SO_ENOTDIR (-57)
#define SO_ENOTEMPTY (-58)
#define SO_ENOTSOCK (-59) // Not a socket
#define SO_ENOTSUP (-60)
#define SO_ENOTTY (-61)
#define SO_ENXIO (-62)      // Network interface device doesn't exist
#define SO_EOPNOTSUPP (-63) // Non-supported operation
#define SO_EOVERFLOW (-64)
#define SO_EPERM (-65)
#define SO_EPIPE (-66)
#define SO_EPROTO (-67)
#define SO_EPROTONOSUPPORT (-68) // Non-supported protocol
#define SO_EPROTOTYPE (-69)      // Non-supported socket type
#define SO_ERANGE (-70)
#define SO_EROFS (-71)
#define SO_ESPIPE (-72)
#define SO_ESRCH (-73)
#define SO_ESTALE (-74)
#define SO_ETIME (-75)
#define SO_ETIMEDOUT (-76) // Timed out
#define SO_ETXTBSY (-77)
#define SO_EWOULDBLOCK SO_EAGAIN // Posix.1g
#define SO_EXDEV (-78)
#define SO_ERR_DHCP_TIMEOUT (-100) // Could not find any DHCP server.
#define SO_ERR_DHCP_EXPIRED (-101)
#define SO_ERR_DHCP_NAK (-102)
#define SO_ERR_LCP (-107)  // LCP negotiation error
#define SO_ERR_AUTH (-108) // Authentication error
#define SO_ERR_IPCP (-109) // IPCP negotiation error
#define SO_ERR_ADDR_COLLISION (-111) // Duplicate IP address
#define SO_ERR_LINK_DOWN (-112)      // Link down
#define SO_ERR_LINK_UP_TIMEOUT (-121) // Link-up timeout

#define SO_MEM_ADDRINFO 10
#define SO_MEM_RESOLVER 11
#define SO_MEM_IPC 12
#define SO_MEM_RECVFROM 13
#define SO_MEM_SENDTO 14

/* SOStartup() timeout default settings */
#define SO_STARTUP_TIMEOUT 60000

/* Retry count of link-up */
#define SO_STARTUP_LINK_UP_RETRY_COUNT 4

#define SO_SNDBUF 0x1001   /* send buffer size */
#define SO_RCVBUF 0x1002   /* receive buffer size */
#define SO_SNDLOWAT 0x1003 /* send low-water mark */
#define SO_RCVLOWAT 0x1004 /* receive low-water mark */
#define SO_SNDTIMEO 0x1005 /* send timeout */
#define SO_RCVTIMEO 0x1006 /* receive timeout */
#define SO_ERROR 0x1007    /* get error status and clear */
#define SO_TYPE 0x1008     /* get socket type */

/*---------------------------------------------------------------------------*
    Structures
 *---------------------------------------------------------------------------*/
struct timeval
{
    long tv_sec;  /* seconds */
    long tv_usec; /* and microseconds */
};

/* Allocator type definition */
typedef void* (*SOAlloc)(u32 name, s32 size);
typedef void  (*SOFree)(u32 name, void *ptr, s32 size);
/*
 * Notice !
 * Allocator must supply 32-byte aligned buffer.
 */

/* Config of SOInit() */
typedef struct SOLibraryConfig
{
    SOAlloc alloc;
    SOFree free;

} SOLibraryConfig;

/*---------------------------------------------------------------------------*
    Functions
 *---------------------------------------------------------------------------*/

int SOInit(SOLibraryConfig *config);
int SOFinish(void);
int SOStartup(void);
int SOStartupEx(int timeOut);
int SOCleanup(void);

#define F_DUPFD 0 /* Duplicate file descriptor.  */
#define F_GETFD 1 /* Get file descriptor flags.  */
#define F_SETFD 2 /* Set file descriptor flags.  */
#define F_GETFL 3 /* Get file status flags.  */
#define F_SETFL 4 /* Set file status flags.  */

#define O_NONBLOCK 04000U

int SOFcntl(s32 s, u32 cmd, u32 flags);
int SOShutdown(s32 s, u32 how);

void msleep(u32 ms);

// domain = 2, type = 1, protocol = 0
int SOSocket2(int domain, int type, int protocol);


struct sockaddr
{
    u8 sa_len;      // 0
    u8 sa_family;   // 1
    s8 sa_data[14]; // 2
};
struct in_addr
{
    u32 s_addr;
};
struct sockaddr_in
{
    u8 sin_len;              // 0
    u8 sin_family;           // 1
    u16 sin_port;            // 2
    struct in_addr sin_addr; // 4
    s8 sin_zero[8];          // 8
};
int SOConnect(int s, struct sockaddr *, u32 socklen);

// flags = 0
int SOSend(s32 s, const void *data, s32 size, u32 flags);

// flags = 4
int SORecv(s32 s, void *mem, s32 len, u32 flags);

int SOClose(s32 s);
struct hostent
{
    char *h_name;       /* official name of host */
    char **h_aliases;   /* alias list */
    u16 h_addrtype;     /* host address type */
    u16 h_length;       /* length of address */
    char **h_addr_list; /* list of addresses from name server */
};

hostent* SOGetHostByName(char* url);
s32 SOSetSockOpt(s32 s, u32 level, u32 optname, const void *optval, u32 optlen);

#pragma endregion
#pragma region nhttpi_soc
s32 NHTTPi_SocOpen(u32 proto);

#pragma endregion
#endif