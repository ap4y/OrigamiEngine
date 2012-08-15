//
//  InputUnitTests.m
//  OrigamiEngine
//
//  Created by ap4y on 8/14/12.
//
//

#import "InputUnitTests.h"

#import "ORGMInputUnit.h"

@interface InputUnitTests ()
@property (retain, nonatomic) ORGMInputUnit* inputUnit;
@end

@implementation InputUnitTests

- (void)setUp {
    [super setUp];
    _inputUnit = [[ORGMInputUnit alloc] init];
    NSURL* flacUrl = [[NSBundle bundleForClass:self.class] URLForResource:@"multiple-vc"
                                                            withExtension:@"flac"];
    STAssertTrue([_inputUnit openWithUrl:flacUrl], nil);
}

- (void)tearDown {
    [_inputUnit close];
    [_inputUnit release];
    [super tearDown];
}

- (void)testInputUnitShouldHaveCorrectAudioFormat {
    AudioStreamBasicDescription format = [_inputUnit format];
    STAssertEquals((NSInteger)format.mFormatID, kAudioFormatLinearPCM, nil);
    STAssertEqualsWithAccuracy(format.mSampleRate, 44100.0, 0.1, nil);
    STAssertEquals((NSInteger)format.mBitsPerChannel, 16, nil);
    STAssertEquals((NSInteger)format.mChannelsPerFrame, 2, nil);
}

- (void)testInputUnitShouldHaveCorrectFramesCount {
    STAssertEqualsWithAccuracy([_inputUnit framesCount], 162496.0, 0.1, nil);
}

- (void)testInputUnitShouldHaveMetadata {
    STAssertEqualObjects([_inputUnit metadata], @{@"artist": @"Artist 2"}, nil);
}

- (void)testInputUnitShouldProcessData {
    [_inputUnit process];
    STAssertEquals(_inputUnit.data.length, 139264U, nil);
}

- (void)testInputUnitShouldSeekToTime {
    [_inputUnit seek:1];
    [_inputUnit process];
    STAssertEquals(_inputUnit.data.length, 132848U, nil);
}

- (void)testInputUnitShouldSetEndFlag {
    [_inputUnit seek:(([_inputUnit framesCount]/44100) - 0.01)];
    [_inputUnit process];
    STAssertTrue(_inputUnit.endOfInput, nil);
}

- (void)testInputUnitShouldNotExceedMaxAmountInBuffer {
    [_inputUnit process];
    NSUInteger _saveLength = _inputUnit.data.length;
    [_inputUnit process];
    STAssertEquals(_inputUnit.data.length, _saveLength, nil);    
}

@end
