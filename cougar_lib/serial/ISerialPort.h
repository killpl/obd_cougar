//
//  Communication.h
//  serial
//
//  Created by Adam Kozłowski on 12/03/2017.
//
//

#include <string>

namespace Cougar
{
    enum class SerialError
    {
        NO_ERROR = -1,
        UNKNOWN,
        
        // Open
        ACCESS_DENIED,
        AGAIN,
        QUOTA,                  //> Should not happen with no "O_CREAT"
        EXIST,                  //> As above ^.
        FAULT,
        INTERRUPTED,
        INVALID_FLAG,           //> One or more of specified flags is invalid.
        INPUT_OUTPUT_ERROR,
        IS_DIRECTORY,           //> Specified path leads to directory, not file.
        NOT_DIRECTORY,          //> Path component is not a directory.
        DEVICE_DISCONNECTED,    //> Path leads to device and associated device does not exist.
        LOCK_NOT_SUPPORTED,     //> Shared or exclusive lock not supported by filesystem.
        OVERFLOW,               //> Filesize too big
        READ_ONLY,              //> File is on read-only filesystem, cannot be modified.
        INVALID_PATH,           //> Invalid path
        LOOP,                   //> Symlinks infinite loop.
        NAME_TOO_LONG,          //> Component of specified path too long.
        FILETABLE_FULL,         //>
        DESCRIPTORS_LIMIT,      //> Process descriptors limit reached.
        
    };

    enum class SerialOpenFlag
    {
        READ_ONLY       = (1u << 0),
        WRITE_ONLY      = (1u << 1),
        READ_WRITE      = (1u << 2),
        NONBLOCK        = (1u << 3),
        APPEND          = (1u << 4),
        SHARED_LOCK     = (1u << 5),
        EXCLUSIVE_LOCK  = (1u << 6),
        NO_FOLLOW       = (1u << 7),
        NO_CTTY         = (1u << 8),
        SYMLINK         = (1u << 9),
        TRUNCATE        = (1u << 10)
    };

    class SerialPort
    {
    public:
        
        SerialPort(std::string& path)
        : _portPath(path)
        {}

        virtual ~SerialPort() {}
        
        virtual SerialError Open(unsigned int flags) = 0;
        virtual void Close() = 0;
        
        int Write(std::string data);
        int Read(std::string& data);
        
        
    protected:
        std::string _portPath;
    };
}
