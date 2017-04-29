//
//  IntefacesOSX.h
//  serial
//
//  Created by Adam Kozłowski on 12/03/2017.
//
//

#pragma once

#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>

#include "../IInterfaces.h"

#include <vector>

namespace Cougar {
    
    class InterfacesOSX : public Interfaces
    {
    public:
        /**
         * Returns list of serial devices available in macOS, retrieved
         * using IOKit.
         */
        virtual std::vector<SerialDevice> GetDevices() override;
        
        virtual ~InterfacesOSX() {}
        
    private:
        ParentDevice GetParentDevice(io_object_t& object);
        
        std::string CFStringToString(CFStringRef input);
        std::string GetDeviceClass(io_object_t& device);
        std::string GetPropertyString(io_object_t& device, const char* key);
        uint GetPropertyInt(io_object_t& device, const char* key);
        std::string GetStringDataForDeviceKey(io_object_t& device, CFStringRef key);
    };
}
