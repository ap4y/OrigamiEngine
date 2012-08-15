//
//  FlacDecoderTests.m
//  OrigamiEngine
//
//  Created by ap4y on 8/15/12.
//
//

#import "FlacDecoderTests.h"

#import "FlacDecoder.h"
#import "FileSource.h"

@interface FlacDecoderTests ()
@property (nonatomic, retain) FlacDecoder* decoder;
@end

@implementation FlacDecoderTests

- (void)setUp {
    [super setUp];
    _decoder = [[FlacDecoder alloc] init];
    
    FileSource* source = [[FileSource alloc] init];
    
    NSURL* flacUrl = [[NSBundle bundleForClass:self.class] URLForResource:@"multiple-vc"
                                                            withExtension:@"flac"];
    [source open:flacUrl];

    STAssertTrue([_decoder open:source], nil);
    [source release];
}

- (void)tearDown {
    [_decoder close];
    [_decoder release];
    [super tearDown];
}

- (void)testFlacDecoderShouldReturnSuppertedFileTypes {
    STAssertEqualObjects(@[@"flac"], [FlacDecoder fileTypes], nil);
}

- (void)testFlacDecoderShouldReturnSuppertedValidProperties {
    NSDictionary* properties = [NSDictionary dictionaryWithObjectsAndKeys:
                                [NSNumber numberWithInt:2], @"channels",
                                [NSNumber numberWithInt:16], @"bitsPerSample",
                                [NSNumber numberWithFloat:44100.0], @"sampleRate",
                                [NSNumber numberWithDouble:162496.0], @"totalFrames",
                                [NSNumber numberWithBool:YES], @"seekable",
                                @"big",@"endian",
                                nil];
    STAssertEqualObjects(properties, [_decoder properties], nil);
}

- (void)testFlacDecoderShouldReturnSuppertedValidMetadata {    
    STAssertEqualObjects(@{@"artist": @"Artist 2"}, [_decoder metadata], nil);
}

- (void)testFlacDecoderShouldReadAudioData {
    void* buffer = malloc(100);
    STAssertEquals(100, [_decoder readAudio:buffer frames:100], nil);
    free(buffer);
}

- (void)testFlacDecoderShouldSeekAudioData {
    STAssertEquals(10L, [_decoder seek:10], nil);
}

@end
