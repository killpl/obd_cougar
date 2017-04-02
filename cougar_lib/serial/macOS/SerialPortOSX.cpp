//
//  SerialPortOSX.cpp
//  binding
//
//  Created by Adam Kozłowski on 02/04/2017.
//
//

#include "SerialPortOSX.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <sysexits.h>

using namespace Cougar;

////////////////////////////////////////////////////////////////////////////////

SerialError SerialPortOSX::Open(unsigned int flags)
{
    int descriptor = open(_portPath.c_str(), UnixFlagsForEnumFlags(flags));
    
    if (descriptor >= 0)
    {
        _descriptor = descriptor;
        return SerialError::NO_ERROR;
    }
    
#define MAP(x,y) case (x): return SerialError::y;
    switch (errno)
    {
        MAP(EACCES,     ACCESS_DENIED)
        MAP(EAGAIN,     AGAIN)
        MAP(EDQUOT,     QUOTA)
        MAP(EEXIST,     EXIST)
        MAP(EFAULT,     FAULT)
        MAP(EINTR,      INTERRUPTED)
        MAP(EINVAL,     INVALID_FLAG)
        MAP(EIO,        INPUT_OUTPUT_ERROR)
        MAP(EISDIR,     IS_DIRECTORY)
        MAP(ELOOP,      LOOP)
        MAP(EMFILE,     DESCRIPTORS_LIMIT)
        MAP(ENAMETOOLONG, NAME_TOO_LONG)
        MAP(ENFILE,     FILETABLE_FULL)
        MAP(ENOENT,     INVALID_PATH)
        MAP(ENOTDIR,    INVALID_PATH)
        MAP(ENXIO,      DEVICE_DISCONNECTED)
        MAP(EOPNOTSUPP, LOCK_NOT_SUPPORTED)
        MAP(EOVERFLOW,  OVERFLOW)
        MAP(EBADF,      INVALID_PATH)

        default: ;
    }
#undef MAP
    return SerialError::UNKNOWN;
}

////////////////////////////////////////////////////////////////////////////////

unsigned int SerialPortOSX::UnixFlagsForEnumFlags(int flags)
{
    unsigned int result = 0;
    
#define MAP(x,y) if (flags & (unsigned)SerialOpenFlag::x) result |= y;
    
    MAP(READ_ONLY,  O_RDONLY)
    MAP(WRITE_ONLY, O_WRONLY)
    MAP(READ_WRITE, O_RDWR)
    MAP(NONBLOCK,   O_NONBLOCK)
    MAP(APPEND,     O_APPEND)
    MAP(SHARED_LOCK, O_SHLOCK)
    MAP(EXCLUSIVE_LOCK, O_EXLOCK)
    MAP(NO_FOLLOW,  O_NOFOLLOW)
    MAP(NO_CTTY,    O_NOCTTY)
    MAP(SYMLINK,    O_SYMLINK)
    MAP(TRUNCATE,   O_TRUNC)

#undef MAP
    return result;
}

////////////////////////////////////////////////////////////////////////////////

void SerialPortOSX::Close()
{
    if (_descriptor >= 0)
    {
        close(_descriptor);
    }
}

////////////////////////////////////////////////////////////////////////////////

