//
//  CusSheetDecoderTests.m
//  OrigamiEngine
//
//  Created by ap4y on 8/15/12.
//
//

#import "CusSheetDecoderTests.h"

#import "CueSheetDecoder.h"
#import "FileSource.h"

@interface CusSheetDecoderTests ()
@property (nonatomic, retain) CueSheetDecoder* decoder;
@end

@implementation CusSheetDecoderTests

- (void)setUp {
    [super setUp];
    _decoder = [[CueSheetDecoder alloc] init];
    
    FileSource* source = [[FileSource alloc] init];
    
    NSURL* cueUrl = [[NSBundle bundleForClass:self.class] URLForResource:@"multiple-vc"
                                                               withExtension:@"cue"];
    NSString* cuePath = [[cueUrl absoluteString] stringByAppendingString:@"#01"];
    cueUrl = [NSURL URLWithString:cuePath];
    [source open:cueUrl];
    
    STAssertTrue([_decoder open:source], nil);
    [source release];
}

- (void)tearDown {
    [_decoder close];
    [_decoder release];
    [super tearDown];
}

- (void)testFlacDecoderShouldReturnSupportedFileTypes {
    STAssertEqualObjects(@[@"cue"], [CueSheetDecoder fileTypes], nil);
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
    NSDictionary* metadata = @{
        @"album": @"Banga",
        @"artist": @"Patti Smith",
        @"genre": @"Rock",
        @"title": @"Amerigo",
        @"track": @1,
        @"year": @"2012",
    };
    STAssertEqualObjects(metadata, [_decoder metadata], nil);
}

- (void)testFlacDecoderShouldReadAudioData {
    void* buffer = malloc(16 * 1024);
    STAssertEquals(4, [_decoder readAudio:buffer frames:4], nil);
    free(buffer);
}

- (void)testFlacDecoderShouldSeekAudioData {
    STAssertEquals(10L, [_decoder seek:10], nil);
}

@end
