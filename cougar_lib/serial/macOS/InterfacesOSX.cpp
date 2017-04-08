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
        int len = CFStringGetLength(input)+1; // ASCII, to allow further open() operation

        char* cstr = new char[len];
        Boolean result = CFStringGetCString(input, cstr, len, kCFStringEncodingASCII);
        CFRelease(input);
        
        if (result)
        {
            std::string resultString(cstr, len);
            delete[] cstr;
            return resultString;
        }
        delete[] cstr;
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
        LOGE << METHOD << "Property " << key << " does not exist";
        return result;
    }
    
    if (CFGetTypeID(propertyValue) == CFStringGetTypeID())
    {
        return CFStringToString(static_cast<CFStringRef>(propertyValue));
    }
    else
    {
        LOGE << METHOD << "Property " << key << " is not string type";
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
        LOGE << METHOD << "Property " << key << " does not exist";
        return result;
    }
    
    if (CFGetTypeID(propertyValue) == CFNumberGetTypeID())
    {
        CFNumberGetValue(static_cast<CFNumberRef>(propertyValue), kCFNumberSInt16Type, &result);
    }
    else
    {
        LOGE << METHOD << "Property " << key << " is not integer type";
    }
    
    CFRelease(propertyValue);
    return result;
}

////////////////////////////////////////////////////////////////////////////////

std::string InterfacesOSX::GetStringDataForDeviceKey(io_object_t& device, CFStringRef key)
{
    CFTypeRef resultString = IORegistryEntryCreateCFProperty(device, key, kCFAllocatorDefault, 0);
    return CFStringToString((CFStringRef)resultString);
}

////////////////////////////////////////////////////////////////////////////////

ParentDevice InterfacesOSX::GetParentDevice(io_object_t& object)
{
    ParentDevice device;
    device.type = DeviceType::OTHER;
    
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
            LOGE << METHOD << "Error accessing parent device";
            break;
        }
        
        IOObjectRelease(deviceObject);
        deviceObject = parent;
            
        if (GetDeviceClass(parent) == USB_DEVICE_ID)
        {
            LOGI << METHOD << BLUETOOTH_DEVICE_ID;
            
            device.type = DeviceType::USB_DEVICE;
            
            device.name = GetPropertyString(parent, "USB Product Name");
            device.vendorName = GetPropertyString(parent, "USB Vendor Name");
            device.serialNumber = GetPropertyInt(parent, "USB Serial Number");
            device.vendorId = GetPropertyInt(parent, "idVendor");
            device.productId = GetPropertyInt(parent, "idProduct");
            
            // Log the debug informations about the USB device
            LOGD << METHOD << "USB Product Name:\t" << device.name;
            LOGD << METHOD << "USB Vendor Name:\t" << device.vendorName;
            LOGD << METHOD << "USB Serial Number:\t" << device.serialNumber;
            LOGD << METHOD << "Vendor:\t0x" << std::hex << device.vendorId;
            LOGD << METHOD << "Product:\t0x" << std::hex << device.productId;
            
            break;
        }
        
        if (GetDeviceClass(parent) == BLUETOOTH_DEVICE_ID)
        {
            LOGI << METHOD << BLUETOOTH_DEVICE_ID;
            
            device.type = DeviceType::BLUETOOTH_DEVICE;
            
            device.name = GetPropertyString(parent, "BTTTYName");
            device.channel = GetPropertyInt(parent, "BTRFCOMMChannel");
            device.connectionType = GetPropertyInt(parent, "BTSerialConnectionType");
            
            // Log the debug informations about the bluetooth device
            LOGD << METHOD << "BTRFCOMMChannel:\t" << device.channel; // INT
            LOGD << METHOD << "BTName:\t" << GetPropertyString(parent, "BTName");
            LOGD << METHOD << "BTTTYName:\t" << device.name;
            LOGD << METHOD << "PortDeviceName:\t" << GetPropertyString(parent, "PortDeviceName"); // ?
            LOGD << METHOD << "BTSerialConnectionType:\t" << device.connectionType; // INT
            LOGD << METHOD << "P49SerialPort:\t" << GetPropertyInt(parent, "P49SerialPort"); // INT
            
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
        LOGE << METHOD << "None services matching kIOSerialBSDServiceValue found.";
        return result;
    }
    
    io_iterator_t matchingServices;
    kernResult = IOServiceGetMatchingServices(kIOMasterPortDefault, classesToMatch, &matchingServices);
    
    if (KERN_SUCCESS != kernResult) {
        LOGE << METHOD << "IOServiceGetMatchingServices returned an error: " << kernResult;
        return result;
    }
    
    // Iterate over found devices
    io_object_t serialPort;
    while ((serialPort = IOIteratorNext(matchingServices))) {
        SerialDevice device;
        
        device.name = GetStringDataForDeviceKey(serialPort, CFSTR(kIOTTYDeviceKey));
        device.calloutDevice = GetStringDataForDeviceKey(serialPort, CFSTR(kIOCalloutDeviceKey));
        device.dialinDevice = GetStringDataForDeviceKey(serialPort, CFSTR(kIODialinDeviceKey));
        device.deviceClass = GetDeviceClass(serialPort);
        
        LOGD << METHOD << kIOTTYDeviceKey << ":\t" << device.name;
        LOGD << METHOD << kIOTTYBaseNameKey << ":\t" << GetStringDataForDeviceKey(serialPort, CFSTR(kIOTTYBaseNameKey));
        LOGD << METHOD << kIOTTYSuffixKey << ":\t" << GetStringDataForDeviceKey(serialPort, CFSTR(kIOTTYSuffixKey));
        LOGD << METHOD << kIOCalloutDeviceKey << ":\t" << device.calloutDevice;
        LOGD << METHOD << kIODialinDeviceKey << ":\t" << device.dialinDevice;
        
        LOGI << METHOD << "Class:\t" << device.deviceClass;
        LOGD << METHOD << "";
        
        device.parent = GetParentDevice(serialPort);
        result.push_back(device);
        
        IOObjectRelease(serialPort);
    }
    IOObjectRelease(matchingServices);
    
    return result;
}

////////////////////////////////////////////////////////////////////////////////
