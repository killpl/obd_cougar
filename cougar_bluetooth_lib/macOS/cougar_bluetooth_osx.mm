//
//  cougar_bluetooth_osx.m
//  binding
//
//  Created by Adam Kozłowski on 19/02/2017.
//
//

#import "cougar_bluetooth_osx.h"
#import <CoreBluetooth/CoreBluetooth.h>

#include "../../cougar_lib_commons/Logger.h"

@interface CougarBluetoothOSX() <CBCentralManagerDelegate>

@property (atomic, strong) CBCentralManager *centralManager;
@property (atomic, assign) BOOL scanning;
@property (nonatomic, copy) void (^callbackBlock)(NSString*);

@end

@implementation CougarBluetoothOSX {
    dispatch_queue_t _managerQueue; // I wonder if this is necessary
}

////////////////////////////////////////////////////////////////////////////////

- (instancetype)init {
    if (self = [super init]) {
        
        LOGD << __PRETTY_FUNCTION__;
        
        self.centralManager = [[CBCentralManager alloc] initWithDelegate:self
                                                                   queue:dispatch_get_main_queue()];
    }
    return self;
}

////////////////////////////////////////////////////////////////////////////////

- (void)startScanForDevices:(NSString *)deviceId withCallback:(void (^)(NSString*))block {
    
    LOGD << __PRETTY_FUNCTION__;
    self.callbackBlock = block;
    if ([deviceId isEqualToString:@""]) {
        
        LOGD << __PRETTY_FUNCTION__ << "Scanning for all devices";
    }
    
    [self.centralManager scanForPeripheralsWithServices: ([deviceId isEqualToString:@""] ? nil : @[[CBUUID UUIDWithString:deviceId],])
                                                options:@{ CBCentralManagerScanOptionAllowDuplicatesKey : @YES }];
    _scanning = YES;
}

////////////////////////////////////////////////////////////////////////////////

- (void)stopScan {

    LOGD << __PRETTY_FUNCTION__;
    [_centralManager stopScan];
    self.callbackBlock = nil;
    
    _scanning = NO;
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
    
    LOGD << __PRETTY_FUNCTION__;
}

////////////////////////////////////////////////////////////////////////////////

- (void)centralManager:(CBCentralManager *)central didRetrievePeripherals:(NSArray<CBPeripheral *> *)peripherals {
    
    LOGD << __PRETTY_FUNCTION__;
    
    if (_callbackBlock) {
        for (CBPeripheral *peripheral : peripherals) {
            _callbackBlock(peripheral.name);
            
            LOGD << __PRETTY_FUNCTION__ << [peripheral.name UTF8String];
        }
    }
    else {
        LOGD << __PRETTY_FUNCTION__ << "No callback set";
    }
}

////////////////////////////////////////////////////////////////////////////////

- (void)centralManager:(CBCentralManager *)central didRetrieveConnectedPeripherals:(NSArray<CBPeripheral *> *)peripherals {
    
    LOGD << __PRETTY_FUNCTION__;
}

////////////////////////////////////////////////////////////////////////////////

- (void)centralManager:(CBCentralManager *)central didDiscoverPeripheral:(CBPeripheral *)peripheral advertisementData:(NSDictionary<NSString *, id> *)advertisementData RSSI:(NSNumber *)RSSI {
    
    LOGD << __PRETTY_FUNCTION__;
    
    if (_callbackBlock) {
        _callbackBlock(peripheral.name);
        
        LOGD << __PRETTY_FUNCTION__ << [peripheral.name UTF8String];
    }
    else {
        LOGD << __PRETTY_FUNCTION__ << "No callback set";
    }
}

////////////////////////////////////////////////////////////////////////////////

@end
