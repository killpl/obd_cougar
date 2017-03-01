//
//  cougar_bluetooth.h
//  binding
//
//  Created by Adam Kozłowski on 19/02/2017.
//
//

#pragma once

#include "../cougar_bluetooth_interface.h"
#include "../../cougar_lib_commons/Listenable.h"

#include <memory>
#include <string>

namespace Cougar_Bluetooth {

// Forward declaration
struct PlatformSpecificContainer;
    
/**
 * Wrapper class for native os bluetooth enumeration.
 * Might be shared with iOS.
 */
class CougarBluetooth : public Listenable<CougarBluetoothInterface>
{
public:
    CougarBluetooth(std::shared_ptr<CougarBluetoothInterface> listener);
    virtual ~CougarBluetooth();
    
    void StartScan(std::string deviceUUID);
    
    void StopScan();
    
    CougarBluetoothStatus GetStatus();
    
    bool IsScanning();
    
private:
    /**
     * Holds platform specific implementation of the interface
     * that communicates with bluetooth.
     */
    struct PlatformSpecificContainer *_container;
};
    
} // namespace Cougar_Bluetooth
