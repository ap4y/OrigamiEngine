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
    STAssertEquals(kAudioFormatLinearPCM, (NSInteger)format.mFormatID, nil);
    STAssertEqualsWithAccuracy(44100.0, format.mSampleRate, 0.1, nil);
    STAssertEquals(16, (NSInteger)format.mBitsPerChannel, nil);
    STAssertEquals(2, (NSInteger)format.mChannelsPerFrame, nil);
}

- (void)testInputUnitShouldHaveCorrectFramesCount {
    STAssertEqualsWithAccuracy(162496.0, [_inputUnit framesCount], 0.1, nil);
}

- (void)testInputUnitShouldHaveMetadata {
    STAssertEqualObjects(@{@"artist": @"Artist 2"}, [_inputUnit metadata], nil);
}

- (void)testInputUnitShouldProcessData {
    [_inputUnit process];
    STAssertEquals(139264U, _inputUnit.data.length, nil);
}

- (void)testInputUnitShouldSeekToTime {
    [_inputUnit seek:1];
    [_inputUnit process];
    STAssertEquals(132848U, _inputUnit.data.length, nil);
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
    STAssertEquals(_saveLength, _inputUnit.data.length, nil);    
}

@end
