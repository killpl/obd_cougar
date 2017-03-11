//
//  cougar_bluetooth.cpp
//  binding
//
//  Created by Adam Kozłowski on 19/02/2017.
//
//

#include "../../cougar_lib_commons/Logger.h"

#include "cougar_bluetooth.h"
#include "cougar_bluetooth_osx.h"
#include "binding_utilities.h"

using namespace Cougar_Bluetooth;
using namespace std;


////////////////////////////////////////////////////////////////////////////////

struct Cougar_Bluetooth::PlatformSpecificContainer {
    CougarBluetoothOSX *bluetooth;;
};

////////////////////////////////////////////////////////////////////////////////

CougarBluetooth::CougarBluetooth()
{
    Log::LOGD(__PRETTY_FUNCTION__);
    _container = new PlatformSpecificContainer();
    _container->bluetooth = [[CougarBluetoothOSX alloc] init];
}

////////////////////////////////////////////////////////////////////////////////

CougarBluetooth::~CougarBluetooth()
{
    Log::LOGD(__PRETTY_FUNCTION__);
    if (_container)
    {
        delete _container;
    }
}

////////////////////////////////////////////////////////////////////////////////

void CougarBluetooth::StartScan(std::string deviceUUID, std::function<void(std::string)> resultCallback)
{
    Log::LOGD(__PRETTY_FUNCTION__);
    // The exact block argument type is different, lambda will suffice as conversion
    [_container->bluetooth startScanForDevices:STLStringToNSString(deviceUUID) withCallback:[resultCallback](NSString* s) -> void { resultCallback(NSStringToSTLString(s)); }];
}

////////////////////////////////////////////////////////////////////////////////

void CougarBluetooth::StopScan()
{
    Log::LOGD(__PRETTY_FUNCTION__);
    [_container->bluetooth stopScan];
}

////////////////////////////////////////////////////////////////////////////////

CougarBluetoothStatus CougarBluetooth::GetStatus()
{
    Log::LOGD(__PRETTY_FUNCTION__);
    // Not a perfectly safe cast, but fine for now
    return (CougarBluetoothStatus)[_container->bluetooth status];
}

////////////////////////////////////////////////////////////////////////////////

bool CougarBluetooth::IsScanning()
{
    Log::LOGD(__PRETTY_FUNCTION__);
    return [_container->bluetooth isScanning];
}

////////////////////////////////////////////////////////////////////////////////
