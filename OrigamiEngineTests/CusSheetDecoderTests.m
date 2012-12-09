//
// CusSheetDecoderTests.m
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

#import "CusSheetDecoderTests.h"

#import "CueSheetDecoder.h"
#import "FileSource.h"

@interface CusSheetDecoderTests ()
@property (nonatomic, retain) CueSheetDecoder *decoder;
@end

@implementation CusSheetDecoderTests

- (void)setUp {
    [super setUp];
    _decoder = [[CueSheetDecoder alloc] init];
    
    FileSource *source = [[FileSource alloc] init];
    
    NSURL *cueUrl = [[NSBundle bundleForClass:self.class] URLForResource:@"multiple-vc"
                                                               withExtension:@"cue"];
    NSString *cuePath = [[cueUrl absoluteString] stringByAppendingString:@"#01"];
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
    STAssertEqualObjects([CueSheetDecoder fileTypes], @[@"cue"], nil);
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
    NSDictionary *metadata = @{
        @"album": @"Banga",
        @"artist": @"Patti Smith",
        @"genre": @"Rock",
        @"title": @"Amerigo",
        @"track": @1,
        @"year": @"2012",
    };
    STAssertEqualObjects([_decoder metadata], metadata, nil);
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
