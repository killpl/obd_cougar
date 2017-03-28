#include "InterfacesOSX.h"

#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/serial/IOSerialKeys.h>
#include <IOKit/serial/ioss.h>
#include <IOKit/IOBSD.h>

using namespace Cougar;
using namespace std;

#include "../../../cougar_lib_commons/Logger.h"

#define METHOD __PRETTY_FUNCTION__ << ": "
#define USB_DEVICE_ID "IOUSBDevice"
#define BLUETOOTH_DEVICE_ID "IOBluetoothSerialClient"

////////////////////////////////////////////////////////////////////////////////

std::string InterfacesOSX::CFStringToString(CFStringRef input)
{
    if (input) {
        int len = CFStringGetLength(input)*2+1; // UTF8

        char* cstr = new char[len];
        Boolean result = CFStringGetCString(input,
                                    cstr,
                                    len,
                                    kCFStringEncodingUTF8);
        CFRelease(input);
        if (result) {
            std::string resultString(cstr, len);
            delete[] cstr;
            return resultString;
        }
    }
    return "";
}

////////////////////////////////////////////////////////////////////////////////

std::string InterfacesOSX::GetDeviceClass(io_object_t& device)
{
    std::string result;
    io_name_t name;
    kern_return_t kern_result = IOObjectGetClass(device, name);
    if(kern_result == KERN_SUCCESS)
    {
        result = std::string(name);
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////

std::string InterfacesOSX::GetPropertyString(io_object_t& device, const char* key)
{
    std::string result;
    
    CFStringRef propertyName = CFStringCreateWithCString(kCFAllocatorDefault, key, kCFStringEncodingASCII);
    
    if (propertyName == NULL)
    {
        return result;
    }
    
    CFTypeRef propertyValue = IORegistryEntryCreateCFProperty(device, propertyName, kCFAllocatorDefault, 0);
    CFRelease(propertyName);
    
    if (propertyValue == NULL)
    {
        return result;
    }
    
    if (CFGetTypeID(propertyValue) == CFStringGetTypeID())
    {
        return CFStringToString(static_cast<CFStringRef>(propertyValue));
    }
    
    CFRelease(propertyValue);
    return result;
}

////////////////////////////////////////////////////////////////////////////////

uint InterfacesOSX::GetPropertyInt(io_object_t& device, const char* key)
{
    uint result = 0;
    
    CFStringRef propertyName = CFStringCreateWithCString(kCFAllocatorDefault, key, kCFStringEncodingASCII);
    
    if (propertyName == NULL)
    {
        return result;
    }
    
    CFTypeRef propertyValue = IORegistryEntryCreateCFProperty(device, propertyName, kCFAllocatorDefault, 0);
    CFRelease(propertyName);
    
    if (propertyValue == NULL)
    {
        return result;
    }
    
    if (CFGetTypeID(propertyValue) == CFNumberGetTypeID())
    {
        CFNumberGetValue(static_cast<CFNumberRef>(propertyValue), kCFNumberSInt16Type, &result);
    }
    
    CFRelease(propertyValue);
    return result;
}

////////////////////////////////////////////////////////////////////////////////

std::string InterfacesOSX::GetStringDataForDeviceKey(io_object_t& device, CFStringRef key)
{
    CFTypeRef resultString = IORegistryEntryCreateCFProperty(device,
                                                             key,
                                                             kCFAllocatorDefault,
                                                             0);
    return CFStringToString((CFStringRef)resultString);
    
}

////////////////////////////////////////////////////////////////////////////////

ParentDevice InterfacesOSX::GetParentDevice(io_object_t& object)
{
    ParentDevice device;
    
    io_registry_entry_t parent = 0;
    
    io_object_t deviceObject = object;
    IOObjectRetain(deviceObject);
    
    kern_return_t kern_result = KERN_SUCCESS;
    
    while (kern_result == KERN_SUCCESS)
    {
        kern_result = IORegistryEntryGetParentEntry(deviceObject,
                                                    kIOServicePlane,
                                                    &parent );
        
        if(kern_result != KERN_SUCCESS)
        {
            break;
        }
        
        IOObjectRelease(deviceObject);
        deviceObject = parent;
            
        if (GetDeviceClass(parent) == USB_DEVICE_ID)
        {
            LOGD << GetPropertyString(parent, "USB Product Name");
            LOGD << GetPropertyString(parent, "USB Vendor Name");
            LOGD << GetPropertyString(parent, "USB Serial Number");
            LOGD << GetPropertyString(parent, "idVendor");
            LOGD << GetPropertyString(parent, "idProduct");
            
            break;
        }
        
        if (GetDeviceClass(parent) == BLUETOOTH_DEVICE_ID)
        {
            LOGD << "BTRFCOMMChannel: " << GetPropertyInt(parent, "BTRFCOMMChannel"); // INT
            LOGD << "BTTTYName: " << GetPropertyString(parent, "BTTTYName");
            LOGD << "BTName: " << GetPropertyString(parent, "BTName");
            LOGD << "PortDeviceName: " << GetPropertyString(parent, "PortDeviceName"); // ?
            LOGD << "BTSerialConnectionType: " << GetPropertyInt(parent, "BTSerialConnectionType"); // INT
            LOGD << "P49SerialPort: " << GetPropertyInt(parent, "P49SerialPort"); // INT
            
            break;
        }
    }
    
    IOObjectRelease(deviceObject);
    return device; // RVO
}

////////////////////////////////////////////////////////////////////////////////

std::vector<SerialDevice> InterfacesOSX::GetDevices()
{
    std::vector<SerialDevice> result;
    
    // Get devices iterator
    kern_return_t           kernResult = 0;
    CFMutableDictionaryRef  classesToMatch = IOServiceMatching(kIOSerialBSDServiceValue);
    
    if (classesToMatch == nullptr)
    {
        LOGD << METHOD << "None services matching kIOSerialBSDServiceValue found.";
        return result;
    }
    
    io_iterator_t matchingServices;
    kernResult = IOServiceGetMatchingServices(kIOMasterPortDefault, classesToMatch, &matchingServices);
    
    if (KERN_SUCCESS != kernResult) {
        LOGD << METHOD << "IOServiceGetMatchingServices returned an error: " << kernResult;
        return result;
    }
    
    // Iterate over found devices
    io_object_t serialPort;
    while ((serialPort = IOIteratorNext(matchingServices))) {
        
        LOGD << METHOD << kIOTTYDeviceKey << ": " << GetStringDataForDeviceKey(serialPort, CFSTR(kIOTTYDeviceKey));
        LOGD << METHOD << kIOTTYBaseNameKey << ": " << GetStringDataForDeviceKey(serialPort, CFSTR(kIOTTYBaseNameKey));
        LOGD << METHOD << kIOTTYSuffixKey << ": " << GetStringDataForDeviceKey(serialPort, CFSTR(kIOTTYSuffixKey));
        LOGD << METHOD << kIOCalloutDeviceKey << ": " << GetStringDataForDeviceKey(serialPort, CFSTR(kIOCalloutDeviceKey));
        LOGD << METHOD << kIODialinDeviceKey << ": " << GetStringDataForDeviceKey(serialPort, CFSTR(kIODialinDeviceKey));
        
        LOGD << METHOD << "Class: " << GetDeviceClass(serialPort);
        
        GetParentDevice(serialPort);
        
        LOGD << METHOD << "";
        
        IOObjectRelease(serialPort);
    }
    
    IOObjectRelease(matchingServices);
    
    return result;
}

////////////////////////////////////////////////////////////////////////////////
