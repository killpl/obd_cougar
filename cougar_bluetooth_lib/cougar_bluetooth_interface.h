//
//  cougar_bluetooth_interface.h
//  binding
//
//  Created by Adam Kozłowski on 19/02/2017.
//
//
#ifndef cougar_bluetooth_interface_h
#define cougar_bluetooth_interface_h

#include <vector>
#include <string>

using namespace std;

namespace Cougar_Bluetooth {
    
enum class CougarBluetoothStatus {
    Off,
    On,
    Unauthorized,
    Resetting,
    Unsupported,
    Unknown
};
    
/**
 * This is common interface for bluetooth changes among multiple platforms,
 * for now it is only macOS, but interface should not be in Obj-C .
 */
class CougarBluetoothInterface
{
public:
    virtual void OnBluetoothStatusChanged() = 0;
    
    virtual void OnBluetoothDevicesFound(vector<string> devices) = 0;
};
    
} // namespace Cougar_Bluetooth {

#endif /* cougar_bluetooth_interface_h */
