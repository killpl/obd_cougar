//
//  ViewController.m
//  BluetoothEnumerationTest
//
//  Created by Adam Kozłowski on 09/03/2017.
//  Copyright © 2017 Adam Kozłowski. All rights reserved.
//

#import "ViewController.h"
#import <CoreBluetooth/CoreBluetooth.h>
#import <GameKit/GameKit.h>

@interface ViewController () <CBCentralManagerDelegate>
    @property (nonatomic, strong) CBCentralManager *centralManager;
    @property (weak) IBOutlet NSButton *startButton;
    @property (weak) IBOutlet NSButton *stopButton;
    @property (weak) IBOutlet NSTextField *resultTextView;
    
    @property (nonatomic, strong) GKSession *session;
@end

@implementation ViewController 

- (void)viewDidLoad {
    [super viewDidLoad];
    _centralManager = [[CBCentralManager alloc] initWithDelegate:self queue:dispatch_get_main_queue()];
    [_startButton setAction:@selector(startButtonClick:)];
    [_stopButton setAction:@selector(stopButtonClick:)];
    
    // Let's try with this crap...
    
    _session = [[GKSession alloc] initWithSessionID:@"something.unique.i.use.my.bundle.name" displayName:nil sessionMode:GKSessionModePeer];
}


- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];
}

- (void)startButtonClick:(id)sender {
    if (_centralManager.state == CBCentralManagerStatePoweredOn) {
        NSDictionary *scanningOptions = @{ CBCentralManagerScanOptionAllowDuplicatesKey : @YES };
        [_centralManager scanForPeripheralsWithServices:@[[CBUUID UUIDWithString:@"180A"],] options:scanningOptions];
    }
}

- (void)stopButtonClick:(id)sender {
    [_centralManager stopScan];
}
    
- (void)centralManagerDidUpdateState:(CBCentralManager *)central {
    NSLog(@"Central manager did update state %ld", (long)central.state);
    
    if (central.state != CBCentralManagerStatePoweredOn) {
        NSLog(@"Bluetooth is not ON");
    }
}

- (void)centralManager:(CBCentralManager *)central willRestoreState:(NSDictionary<NSString *, id> *)dict {
    NSLog(@"Central manager will restore state");
}
    
- (void)centralManager:(CBCentralManager *)central didRetrievePeripherals:(NSArray<CBPeripheral *> *)peripherals {
    NSLog(@"Central manager did retrieve peripherials %lu", (unsigned long)peripherals.count);
}
    
- (void)centralManager:(CBCentralManager *)central didRetrieveConnectedPeripherals:(NSArray<CBPeripheral *> *)peripherals {
    NSLog(@"Central manager did retrieve connected peripherials %lu", (unsigned long)peripherals.count);
}

    
- (void)centralManager:(CBCentralManager *)central didDiscoverPeripheral:(CBPeripheral *)peripheral advertisementData:(NSDictionary<NSString *, id> *)advertisementData RSSI:(NSNumber *)RSSI {
    NSLog(@"Central manager did discover peripherial %@ with signal %@", peripheral.name, RSSI);
}

- (void)peripheral:(CBPeripheral *)peripheral didDiscoverServices:(NSError *)error {
    NSLog(@"Did discover service for peripheral %@", peripheral.name);
}
    
@end
