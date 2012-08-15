//
//  EngineTests.h
//  OrigamiEngine
//
//  Created by ap4y on 8/14/12.
//
//

#import <SenTestingKit/SenTestingKit.h>

#import "ORGMEngine.h"

@interface DelegateTester : NSObject <ORGMEngineDelegate>
@property (assign, nonatomic) ORGMEngineState state;
@property (assign, nonatomic) BOOL trackRequested;
@end

@interface EngineTests : SenTestCase

@end
