//
//  Intefaces.h
//  serial
//
//  Created by Adam Kozłowski on 12/03/2017.
//
//

#pragma once

#include <vector>

namespace Cougar {
    
    struct ParentDevice
    {
        
    };
    
    struct SerialDevice
    {
        
        ParentDevice parent;
    };
    
    
    class Interfaces
    {
        /**
         * Returns list of serial devices available on the platform.
         */
        virtual std::vector<SerialDevice> GetDevices() = 0;
    };
    
}
