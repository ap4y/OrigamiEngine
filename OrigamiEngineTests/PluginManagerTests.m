//
// PluginManagerTests.m
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

#import "PluginManagerTests.h"

#import "ORGMPluginManager.h"

#import "HTTPSource.h"
#import "FileSource.h"

#import "FlacDecoder.h"
#import "CoreAudioDecoder.h"
#import "CueSheetDecoder.h"

#import "CueSheetContainer.h"
#import "M3uContainer.h"

@implementation PluginManagerTests

- (void)testManagerShouldReturnErrorForUnknownSource {
    NSURL *url = [NSURL URLWithString:@"http2://mp3.com/test.mp3"];
    NSError *error;
    STAssertNil([[ORGMPluginManager sharedManager] sourceForURL:url error:&error], nil);
    STAssertEquals(error.code, ORGMEngineErrorCodesSourceFailed, nil);
}

- (void)testManagerShouldReturnErrorForUnknownDecoder {
    NSURL *url = [NSURL URLWithString:@"file:///User/test.mp34"];
    id<ORGMSource> source = [[ORGMPluginManager sharedManager] sourceForURL:url error:nil];
    [source open:url];
    NSError *error;
    STAssertNil([[ORGMPluginManager sharedManager] decoderForSource:source error:&error], nil);
    STAssertEquals(error.code, ORGMEngineErrorCodesDecoderFailed, nil);
}

- (void)testManagerShouldReturnErrorForUnknownContainer {
    NSURL *url = [NSURL URLWithString:@"file:///User/test.mp34"];
    NSError *error;
    STAssertNil([[ORGMPluginManager sharedManager] urlsForContainerURL:url error:&error], nil);
    STAssertEquals(error.code, ORGMEngineErrorCodesContainerFailed, nil);
}

- (void)testManagerShouldReturnSourceForHTTPScheme {
    NSURL *url = [NSURL URLWithString:@"http://mp3.com/test.mp3"];
    id<ORGMSource> source = [[ORGMPluginManager sharedManager] sourceForURL:url error:nil];
    STAssertEqualObjects([source class], [HTTPSource class], nil);
}

- (void)testManagerShouldReturnSourceForFileScheme {
    NSURL *url = [NSURL URLWithString:@"file://test.mp3"];
    id<ORGMSource> source = [[ORGMPluginManager sharedManager] sourceForURL:url error:nil];
    STAssertEqualObjects([source class], [FileSource class], nil);
}

- (void)testManagerShouldReturnDecoderForMp3Extension {
    NSURL *url = [NSURL URLWithString:@"file:///User/test.mp3"];
    id<ORGMSource> source = [[ORGMPluginManager sharedManager] sourceForURL:url error:nil];
    [source open:url];
    id<ORGMDecoder> decoder = [[ORGMPluginManager sharedManager] decoderForSource:source error:nil];
    STAssertEqualObjects([decoder class], [CoreAudioDecoder class], nil);
}

- (void)testManagerShouldReturnDecoderForFlacExtension {
    NSURL *url = [NSURL URLWithString:@"file:///User/test.flac"];
    id<ORGMSource> source = [[ORGMPluginManager sharedManager] sourceForURL:url error:nil];
    [source open:url];
    id<ORGMDecoder> decoder = [[ORGMPluginManager sharedManager] decoderForSource:source error:nil];
    STAssertEqualObjects([decoder class], [FlacDecoder class], nil);
}

- (void)testManagerShouldReturnDecoderForCueExtension {
    NSURL *url = [NSURL URLWithString:@"file:///User/test.cue"];
    id<ORGMSource> source = [[ORGMPluginManager sharedManager] sourceForURL:url error:nil];
    [source open:url];
    id<ORGMDecoder> decoder = [[ORGMPluginManager sharedManager] decoderForSource:source error:nil];
    STAssertEqualObjects([decoder class], [CueSheetDecoder class], nil);
}

- (void)testManagerShouldReturnUrlsFromCueFile {
    NSURL *url = [[NSBundle bundleForClass:[self class]] URLForResource:@"multi_file"
                                                          withExtension:@"cue"];

    NSArray *urls = [[ORGMPluginManager sharedManager] urlsForContainerURL:url error:nil];
    STAssertEquals(urls.count, 12U, nil);
}

- (void)testManagerShouldReturnUrlsFromM3uFile {
    NSURL *url = [[NSBundle bundleForClass:[self class]] URLForResource:@"test"
                                                          withExtension:@"m3u"];
    
    NSArray *urls = [[ORGMPluginManager sharedManager] urlsForContainerURL:url error:nil];
    STAssertEquals(urls.count, 13U, nil);
}

@end
