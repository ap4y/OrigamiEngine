//
//  EngineTests.m
//  OrigamiEngine
//
//  Created by ap4y on 8/14/12.
//
//

#import "EngineTests.h"

@implementation DelegateTester

- (NSURL *)engineIsExpectNextUrl:(ORGMEngine *)engine {
    _trackRequested = YES;
    return nil;
}

- (void)engine:(ORGMEngine *)engine didChangeState:(ORGMEngineState)state {
    self.state = state;
}

@end

@interface EngineTests ()
@property (retain, nonatomic) ORGMEngine* engine;
@property (retain, nonatomic) DelegateTester* tester;
@end

@implementation EngineTests

- (void)setUp {
    [super setUp];
    _engine = [[ORGMEngine alloc] init]; //unable to create from setup
    _tester = [[DelegateTester alloc] init];
    _engine.delegate = _tester;
    
    NSURL* flacUrl = [[NSBundle bundleForClass:self.class] URLForResource:@"multiple-vc"
                                                            withExtension:@"flac"];
    [_engine playUrl:flacUrl];
}

- (void)tearDown {
    [_engine release];
    [_tester release];
    [super tearDown];
}

- (void)testEngineDelegateShouldReturnState {
    NSDate *loopUntil = [NSDate dateWithTimeIntervalSinceNow:0.1]; //delegate async dispatched to the main thread
    while ([loopUntil timeIntervalSinceNow] > 0) {
        [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode
                                 beforeDate:loopUntil];
    }
    STAssertEquals(_tester.state, ORGMEngineStatePlaying, nil);
}

- (void)testEngineDelegateShouldRequestNextTrackFromDelegate {
    [_engine seekToTime:[_engine trackTime]];
    NSDate *loopUntil = [NSDate dateWithTimeIntervalSinceNow:1.0];
    while ([loopUntil timeIntervalSinceNow] > 0) {
        [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode
                                 beforeDate:loopUntil];
    }
    STAssertTrue(_tester.trackRequested, nil);
}

@end
