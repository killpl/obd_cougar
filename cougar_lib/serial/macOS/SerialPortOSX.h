//
//  SerialPortOSX.hpp
//  binding
//
//  Created by Adam Kozłowski on 02/04/2017.
//
//

#pragma once

#include "../ISerialPort.h"

#include <termios.h>

namespace Cougar {

    class SerialPortOSX : public SerialPort
    {
        
        virtual SerialError Open(unsigned int flags);
        virtual void Close();
        
    protected:
        unsigned int UnixFlagsForEnumFlags(int flags);
        
    private:
        int _descriptor = -1;
        //struct termios _options = { 0,0,0,0,{0},0,0 }; // Fill with zeros
    };

}
