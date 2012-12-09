//
// FileSourceTests.m
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

#import "FileSourceTests.h"

#import "FileSource.h"

@interface FileSourceTests ()
@property (retain, nonatomic) FileSource *source;
@end

@implementation FileSourceTests

- (void)setUp {
    [super setUp];
    
    _source = [[FileSource alloc] init];
    NSURL *flacUrl = [[NSBundle bundleForClass:self.class] URLForResource:@"multiple-vc"
                                                            withExtension:@"flac"];
    STAssertTrue([_source open:flacUrl], nil);
}

- (void)tearDown {
    [_source close];
    [_source release];
    [super tearDown];
}

- (void)testFileSourceShouldReturnValidScheme {
    STAssertEqualObjects([FileSource scheme], @"file", nil);
}

- (void)testFileSourceShouldReturnValidUrl {
    NSURL *flacUrl = [[NSBundle bundleForClass:self.class] URLForResource:@"multiple-vc"
                                                            withExtension:@"flac"];
    STAssertEqualObjects([_source url], flacUrl, nil);
}

- (void)testFileSourceShouldReturnCorrectSize {
    STAssertEquals([_source size], 4754L, nil);
}

- (void)testFileSourceShouldSeekToPositionWithWhenceAndTellCurrentPosition {
    STAssertTrue([_source seek:100 whence:SEEK_SET], nil);
    STAssertEquals([_source tell], 100L, nil);
    STAssertTrue([_source seek:100 whence:SEEK_CUR], nil);
    STAssertEquals([_source tell], 200L, nil);
    STAssertTrue([_source seek:0 whence:SEEK_END], nil);
    STAssertEquals([_source tell], 4754L, nil);
}

- (void)testFileSourceShouldReadData {
    void *buffer = malloc(100);
    STAssertEquals([_source read:buffer amount:100], 100, nil);
    free(buffer);
}

@end
