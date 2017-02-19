//
//  cougar_bluetooth_osx.m
//  binding
//
//  Created by Adam Kozłowski on 19/02/2017.
//
//

#import "cougar_bluetooth_osx.h"
#import <CoreBluetooth/CoreBluetooth.h>

@interface CougarBluetoothOSX() <CBCentralManagerDelegate>

@property (atomic, strong) CBCentralManager *centralManager;
@property (atomic, assign) BOOL scanning;

@end

@implementation CougarBluetoothOSX {
    dispatch_queue_t _managerQueue; // I wonder if this is necessary
}

////////////////////////////////////////////////////////////////////////////////

- (instancetype)init {
    if (self = [super init]) {
        self.centralManager = [[CBCentralManager alloc] initWithDelegate:self
                                                                   queue:dispatch_get_main_queue()];
    }
    return self;
}

////////////////////////////////////////////////////////////////////////////////

- (void)startScanForDevices:(NSString *)deviceId withBlock:(void (NSArray*))block {
    
}

////////////////////////////////////////////////////////////////////////////////

- (void)stopScan {
    [_centralManager stopScan];
}

////////////////////////////////////////////////////////////////////////////////
// Getters
////////////////////////////////////////////////////////////////////////////////

- (enum CougarBluetoothState)status {
    switch ([_centralManager state]) {
        case CBCentralManagerStatePoweredOn:
            return CougarBluetoothStateOn;
        case CBCentralManagerStatePoweredOff:
            return CougarBluetoothStateOff;
        case CBCentralManagerStateResetting:
            return CougarBluetoothStateResetting;
        case CBCentralManagerStateUnauthorized:
            return CougarBluetoothStateUnauthorized;
        case CBCentralManagerStateUnsupported:
            return CougarBluetoothStateUnknown;
        case CBCentralManagerStateUnknown:;
            // This (as opposite of default here) triggers build failure or
            // at least warning, which might indicate changes in the library.
    }
    return CougarBluetoothStateUnknown;
}

////////////////////////////////////////////////////////////////////////////////

- (BOOL)isScanning {
    return _scanning;
}

////////////////////////////////////////////////////////////////////////////////
// CBCentralManagerDelegate
////////////////////////////////////////////////////////////////////////////////

- (void)centralManagerDidUpdateState:(CBCentralManager *)central {
    
}

////////////////////////////////////////////////////////////////////////////////

- (void)centralManager:(CBCentralManager *)central didRetrievePeripherals:(NSArray<CBPeripheral *> *)peripherals {
    
}

////////////////////////////////////////////////////////////////////////////////

- (void)centralManager:(CBCentralManager *)central didRetrieveConnectedPeripherals:(NSArray<CBPeripheral *> *)peripherals {
    
}

////////////////////////////////////////////////////////////////////////////////

- (void)centralManager:(CBCentralManager *)central didDiscoverPeripheral:(CBPeripheral *)peripheral advertisementData:(NSDictionary<NSString *, id> *)advertisementData RSSI:(NSNumber *)RSSI {
    
}

////////////////////////////////////////////////////////////////////////////////

@end
