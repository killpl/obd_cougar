//
//  Tests.m
//  Tests
//
//  Created by Adam Kozłowski on 19/02/2017.
//
//

#import <XCTest/XCTest.h>
#include "../macOS/cougar_bluetooth_osx.h"

@interface Tests : XCTestCase
@property (nonatomic, strong) CougarBluetoothOSX *cougarBluetooth;
@end

@implementation Tests

- (void)setUp {
    [super setUp];
    _cougarBluetooth = [[CougarBluetoothOSX alloc] init];
}

- (void)tearDown {
    [_cougarBluetooth stopScan];
    _cougarBluetooth = nil;
    [super tearDown];
}

- (void)testScan {
    [_cougarBluetooth startScanForDevices:@"none" withBlock:nil];
}

- (void)testStatus {
    CougarBluetoothState status = [_cougarBluetooth status];
}

@end
