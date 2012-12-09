//
// CueSheetTests.m
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

#import "CueSheetTests.h"
#import "CueSheet.h"

@interface CueSheetTests () {
    NSURL *_multiFileCueUrl;
    NSURL *_singleFileCueUrl;
}
@end

@implementation CueSheetTests

- (void)setUp {
    [super setUp];
    _multiFileCueUrl = [[[NSBundle bundleForClass:[self class]] URLForResource:@"multi_file"
                                                                  withExtension:@"cue"] retain];
    _singleFileCueUrl = [[[NSBundle bundleForClass:[self class]] URLForResource:@"single_file"
                                                                 withExtension:@"cue"] retain];
}

- (void)tearDown {
    [_multiFileCueUrl release];
    [_singleFileCueUrl release];
    [super tearDown];
}

- (void)testCueSheetShouldParseCueSheetFromSingleFileFixture {
    CueSheet *cueSheet = [[CueSheet alloc] initWithURL:_singleFileCueUrl];
    
    STAssertNotNil(cueSheet, nil);
    STAssertEquals(cueSheet.tracks.count, 14U, nil);

    NSURL *bundleUrl = [NSBundle bundleForClass:[self class]].bundleURL;
    NSURL *url = [bundleUrl URLByAppendingPathComponent:@"NEW!.flac"];
    
    CueSheetTrack *firstTrack = [cueSheet.tracks objectAtIndex:0];
    STAssertEqualObjects(firstTrack.title, @"Warning", nil);
    STAssertEqualObjects(firstTrack.artist, @"LAMA", nil);
    STAssertEqualObjects(firstTrack.album, @"NEW!", nil);
    STAssertEqualObjects(firstTrack.track, @"01", nil);
    STAssertEqualObjects(firstTrack.year, @"2011", nil);
    STAssertEqualObjects(firstTrack.genre, @"Rock", nil);
    STAssertEqualsWithAccuracy(firstTrack.time, 0.0, 0.1, nil);
    STAssertEqualObjects(firstTrack.url, url, nil);
    
    CueSheetTrack *lastTrack = [cueSheet.tracks objectAtIndex:13];
    STAssertEqualObjects(lastTrack.title, @"Dreamin'", nil);
    STAssertEqualObjects(lastTrack.artist, @"LAMA", nil);
    STAssertEqualObjects(lastTrack.album, @"NEW!", nil);
    STAssertEqualObjects(lastTrack.track, @"14", nil);
    STAssertEqualObjects(lastTrack.year, @"2011", nil);
    STAssertEqualObjects(lastTrack.genre, @"Rock", nil);
    STAssertEqualsWithAccuracy(lastTrack.time, 2691.7, 0.1, nil);
    STAssertEqualObjects(lastTrack.url, url, nil);
}

- (void)testCueSheetShouldParseCueSheetFromMultiFileFixture {
    CueSheet *cueSheet = [[CueSheet alloc] initWithURL:_multiFileCueUrl];
    
    STAssertNotNil(cueSheet, nil);
    STAssertEquals(cueSheet.tracks.count, 12U, nil);
    
    NSURL *bundleUrl = [NSBundle bundleForClass:[self class]].bundleURL;
    
    CueSheetTrack *firstTrack = [cueSheet.tracks objectAtIndex:0];
    STAssertEqualObjects(firstTrack.title, @"Amerigo", nil);
    STAssertEqualObjects(firstTrack.artist, @"Patti Smith", nil);
    STAssertEqualObjects(firstTrack.album, @"Banga", nil);
    STAssertEqualObjects(firstTrack.track, @"01", nil);
    STAssertEqualObjects(firstTrack.year, @"2012", nil);
    STAssertEqualObjects(firstTrack.genre, @"Rock", nil);
    STAssertEqualsWithAccuracy(firstTrack.time, 0.0, 0.1, nil);
    STAssertEqualObjects(firstTrack.url,
                         [bundleUrl URLByAppendingPathComponent:@"01 - Amerigo.wav"],
                         nil);
    
    CueSheetTrack *lastTrack = [cueSheet.tracks objectAtIndex:11];
    STAssertEqualObjects(lastTrack.title, @"After The Gold Rush", nil);
    STAssertEqualObjects(lastTrack.artist, @"Patti Smith", nil);
    STAssertEqualObjects(lastTrack.album, @"Banga", nil);
    STAssertEqualObjects(lastTrack.track, @"12", nil);
    STAssertEqualObjects(lastTrack.year, @"2012", nil);
    STAssertEqualObjects(lastTrack.genre, @"Rock", nil);
    STAssertEqualsWithAccuracy(lastTrack.time, 0.0, 0.1, nil);
    STAssertEqualObjects(lastTrack.url,
                         [bundleUrl URLByAppendingPathComponent:@"12 - After The Gold Rush.wav"],                         
                         nil);
}

@end
