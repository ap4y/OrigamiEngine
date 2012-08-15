//
//  CoreAudioDecoderTests.m
//  OrigamiEngine
//
//  Created by ap4y on 8/15/12.
//
//

#import "CoreAudioDecoderTests.h"

#import "CoreAudioDecoder.h"
#import "FileSource.h"

@interface CoreAudioDecoderTests ()
@property (nonatomic, retain) CoreAudioDecoder* decoder;
@end

@implementation CoreAudioDecoderTests

- (void)setUp {
    [super setUp];
    _decoder = [[CoreAudioDecoder alloc] init];
    
    FileSource* source = [[FileSource alloc] init];
    
    NSURL* flacUrl = [[NSBundle bundleForClass:self.class] URLForResource:@"id3v22-tda"
                                                            withExtension:@"mp3"];
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
    STAssertTrue([[CoreAudioDecoder fileTypes] containsObject:@"mp3"], nil);
    STAssertTrue([[CoreAudioDecoder fileTypes] containsObject:@"wav"], nil);
    STAssertTrue([[CoreAudioDecoder fileTypes] containsObject:@"m4a"], nil);
    STAssertTrue([[CoreAudioDecoder fileTypes] containsObject:@"mp4"], nil);    
}

- (void)testFlacDecoderShouldReturnSuppertedValidProperties {
    NSDictionary* properties = [NSDictionary dictionaryWithObjectsAndKeys:
                                [NSNumber numberWithInt:0], @"bitrate",
                                [NSNumber numberWithInt:2], @"channels",
                                [NSNumber numberWithInt:16], @"bitsPerSample",
                                [NSNumber numberWithFloat:44100.0], @"sampleRate",
                                [NSNumber numberWithDouble:9797760.0], @"totalFrames",
                                [NSNumber numberWithBool:YES], @"seekable",
                                @"big",@"endian",
                                nil];
    STAssertEqualObjects(properties, [_decoder properties], nil);
}

- (void)testFlacDecoderShouldReturnSuppertedValidMetadata {
    NSDictionary* metadata = @{
        @"album": @"n Water I Can Fly",
        @"approximate duration in seconds": @"222.171",
        @"artist": @"Basshunter",
        @"comments": @"Ripped by THSLIVE",
        @"genre": @"Dance",
        @"title": @"I Can Walk On Water I Can Fly",
        @"track number": @"1",
        @"year": @"2010",
    };
    STAssertEqualObjects(metadata, [_decoder metadata], nil);
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
