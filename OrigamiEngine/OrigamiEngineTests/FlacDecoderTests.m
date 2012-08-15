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

- (void)testFlacDecoderShouldReturnSupportedFileTypes {
    STAssertEqualObjects([FlacDecoder fileTypes], @[@"flac"], nil);
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
    STAssertEqualObjects([_decoder properties], properties, nil);
}

- (void)testFlacDecoderShouldReturnSuppertedValidMetadata {    
    STAssertEqualObjects([_decoder metadata], @{@"artist": @"Artist 2"}, nil);
}

- (void)testFlacDecoderShouldReadAudioData {
    void* buffer = malloc(16 * 1024);
    STAssertEquals([_decoder readAudio:buffer frames:4], 4, nil);
    free(buffer);
}

- (void)testFlacDecoderShouldSeekAudioData {
    STAssertEquals([_decoder seek:10], 10L, nil);
}

@end
