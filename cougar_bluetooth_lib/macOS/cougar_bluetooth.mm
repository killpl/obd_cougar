//
//  cougar_bluetooth.cpp
//  binding
//
//  Created by Adam Kozłowski on 19/02/2017.
//
//

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

CougarBluetooth::CougarBluetooth(shared_ptr<CougarBluetoothInterface> listener)
{
    _container->bluetooth = [[CougarBluetoothOSX alloc] init];
}

////////////////////////////////////////////////////////////////////////////////

CougarBluetooth::~CougarBluetooth()
{
    if (_container)
    {
        delete _container;
    }
}

////////////////////////////////////////////////////////////////////////////////

void CougarBluetooth::StartScan(string deviceUUID)
{
    //_container->bluetooth startScanForDevices: withBlock:<#(void (*)(NSArray *))#>
}

////////////////////////////////////////////////////////////////////////////////

void CougarBluetooth::StopScan()
{
    [_container->bluetooth stopScan];
}

////////////////////////////////////////////////////////////////////////////////

CougarBluetoothStatus CougarBluetooth::GetStatus()
{
    // Not a perfectly safe cast, but fine for now
    return (CougarBluetoothStatus)[_container->bluetooth status];
}

////////////////////////////////////////////////////////////////////////////////

bool CougarBluetooth::IsScanning()
{
    return [_container->bluetooth isScanning];
}

////////////////////////////////////////////////////////////////////////////////
