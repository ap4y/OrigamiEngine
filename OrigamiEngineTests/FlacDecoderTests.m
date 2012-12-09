//
// FlacDecoderTests.m
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

#import "FlacDecoderTests.h"

#import "FlacDecoder.h"
#import "FileSource.h"

@interface FlacDecoderTests ()
@property (nonatomic, retain) FlacDecoder *decoder;
@end

@implementation FlacDecoderTests

- (void)setUp {
    [super setUp];
    _decoder = [[FlacDecoder alloc] init];
    
    FileSource *source = [[FileSource alloc] init];
    
    NSURL *flacUrl = [[NSBundle bundleForClass:self.class] URLForResource:@"multiple-vc"
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
    NSDictionary *properties = [NSDictionary dictionaryWithObjectsAndKeys:
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
    void *buffer = malloc(16 * 1024);
    STAssertEquals([_decoder readAudio:buffer frames:4], 4, nil);
    free(buffer);
}

- (void)testFlacDecoderShouldSeekAudioData {
    STAssertEquals([_decoder seek:10], 10L, nil);
}

@end
