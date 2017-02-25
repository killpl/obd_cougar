//
//  cougar_bluetooth_osx.h
//  binding
//
//  Created by Adam Kozłowski on 19/02/2017.
//
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSUInteger, CougarBluetoothState) {
    CougarBluetoothStateOff,
    CougarBluetoothStateOn,
    CougarBluetoothStateUnauthorized,
    CougarBluetoothStateResetting,
    CougarBluetoothStateUnsupported,
    CougarBluetoothStateUnknown
};

@interface CougarBluetoothOSX : NSObject

@property (nonatomic, readonly) enum CougarBluetoothState status;
@property (assign, readonly) BOOL isScanning;

- (instancetype)init;

- (void)startScanForDevices:(NSString *)deviceId withBlock:(void (NSArray*))block;

- (void)stopScan;

@end
