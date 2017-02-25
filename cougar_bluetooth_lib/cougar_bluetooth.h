//
//  cougar_bluetooth.h
//  binding
//
//  Created by Adam Kozłowski on 19/02/2017.
//
//

#pragma once

#include "cougar_bluetooth_interface.h"

#include <memory>
#include <string>

using namespace std;

namespace Cougar_Bluetooth {

// Forward declaration
struct PlatformSpecificContainer;
    
/**
 * Common wrapper class for native system files.
 */
class CougarBluetooth
{
public:
    CougarBluetooth(unique_ptr<CougarBluetoothInterface> listener);
    virtual ~CougarBluetooth();
    
    void StartScan(string deviceUUID);
    
    void StopScan();
    
    CougarBluetoothStatus GetStatus();
    
    bool IsScanning();
    
private:
    // Holds platform specific implementation of the interface
    // that communicates with bluetooth.
    struct PlatformSpecificContainer *_container;
};
    
} // namespace Cougar_Bluetooth
