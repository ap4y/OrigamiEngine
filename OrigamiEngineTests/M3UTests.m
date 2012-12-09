//
// M3UTests.m
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

#import "M3UTests.h"
#import "M3uContainer.h"

@implementation M3UTests

- (void)testM3uContainerShouldParseM3uFileFixture {
    NSURL *m3uUrl = [[NSBundle bundleForClass:[self class]] URLForResource:@"test"
                                                             withExtension:@"m3u"];
    NSArray *fileUrls = [M3uContainer urlsForContainerURL:m3uUrl];
    
    STAssertNotNil(fileUrls, nil);
    STAssertEquals(fileUrls.count, 13U, nil);
    
    NSURL *bundleUrl = [NSBundle bundleForClass:[self class]].bundleURL;
    
    NSURL *firstUrl = [fileUrls objectAtIndex:0];
    STAssertEqualObjects(firstUrl,
                         [bundleUrl URLByAppendingPathComponent:@"01 - Lost in the Echo.flac"],
                         nil);
    
    NSURL *lastUrl = [fileUrls objectAtIndex:12];
    STAssertEqualObjects(lastUrl, 
                         [bundleUrl URLByAppendingPathComponent:@"13 - What I've Done (Live).flac"],
                         nil);
}

@end
