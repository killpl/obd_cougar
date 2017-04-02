//
//  Intefaces.h
//  serial
//
//  Created by Adam Kozłowski on 12/03/2017.
//
//

#pragma once

#include <vector>
#include <string>

namespace Cougar {
    
////////////////////////////////////////////////////////////////////////////////

    enum class DeviceType
    {
        USB_DEVICE,
        BLUETOOTH_DEVICE
    };
    
////////////////////////////////////////////////////////////////////////////////

    struct ParentDevice
    {
        // COMMON
        std::string name;           //> Device name (BTName, USB Product Name)
        DeviceType type;            //> Device type, also indicates possibly filled fields
                                    //  for parent device - USB or Bluetooth.
        
        // BLUETOOTH
        uint32_t channel;           //> Bluetooth channel number
        uint32_t connectionType;    //> Serial device type
        
        // USB
        uint32_t vendorId;          //> USB vendor ID
        uint32_t productId;         //> USB product ID
        uint32_t serialNumber;      //> USB device serial number
        
        std::string vendorName;     //> USB vendor name string
        
    };
    
////////////////////////////////////////////////////////////////////////////////

    struct SerialDevice
    {
        std::string name;           //> Serial device name
        std::string deviceClass;    //> In most cases: IOSerialBSDClient
        
        std::string calloutDevice;  //> UNIX serial callout device (serial port in /dev/tty...)
        std::string dialinDevice;   //> UNIX serial dialin device
        
        ParentDevice parent;        //> Parent device, either USB or Bluetooth supported
    };
    
////////////////////////////////////////////////////////////////////////////////
    
    class Interfaces
    {
        /**
         * Returns list of serial devices available on the platform
         as a vector of @{SerialDevice} objects.
         */
        virtual std::vector<SerialDevice> GetDevices() = 0;
    };
    
////////////////////////////////////////////////////////////////////////////////
}
