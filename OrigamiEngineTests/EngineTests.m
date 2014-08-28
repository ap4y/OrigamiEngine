//
// EngineTests.m
//
// Copyright (c) 2012 ap4y (lod@pisem.net)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#import "EngineTests.h"

@implementation DelegateTester

- (NSURL *)engineExpectsNextUrl:(ORGMEngine *)engine {
    _trackRequested = YES;
    return nil;
}

- (void)engine:(ORGMEngine *)engine didChangeState:(ORGMEngineState)state {
    self.state = state;
}

@end

@interface EngineTests ()
@property (retain, nonatomic) ORGMEngine *engine;
@property (retain, nonatomic) DelegateTester *tester;
@end

@implementation EngineTests

- (void)setUp {
    [super setUp];
    _engine = [[ORGMEngine alloc] init]; //unable to create from setup
    _tester = [[DelegateTester alloc] init];
    _engine.delegate = _tester;
    
    NSURL *flacUrl = [[NSBundle bundleForClass:self.class] URLForResource:@"multiple-vc"
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
    NSDate *loopUntil = [NSDate dateWithTimeIntervalSinceNow:3.0];
    while ([loopUntil timeIntervalSinceNow] > 0) {
        [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode
                                 beforeDate:loopUntil];
    }
    STAssertTrue(_tester.trackRequested, nil);
}

- (void)testEngineShouldChangeStateToStoppedIfNoNextTrackIsGiven {
    NSDate *loopUntil = [NSDate dateWithTimeIntervalSinceNow:3.0];
    while ([loopUntil timeIntervalSinceNow] > 0) {
        [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode
                                 beforeDate:loopUntil];
    }
    STAssertTrue(_engine.currentState, ORGMEngineStateStopped);
}


@end
