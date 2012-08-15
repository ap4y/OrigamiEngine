//
//  M3UTests.m
//  OrigamiEngine
//
//  Created by ap4y on 8/10/12.
//
//

#import "M3UTests.h"
#import "M3uContainer.h"

@implementation M3UTests

- (void)testM3uContainerShouldParseM3uFileFixture {
    NSURL* m3uUrl = [[NSBundle bundleForClass:[self class]] URLForResource:@"test"
                                                             withExtension:@"m3u"];
    NSArray* fileUrls = [M3uContainer urlsForContainerURL:m3uUrl];
    
    STAssertNotNil(fileUrls, nil);
    STAssertEquals(fileUrls.count, 13U, nil);
    
    NSURL* bundleUrl = [NSBundle bundleForClass:[self class]].bundleURL;
    
    NSURL* firstUrl = [fileUrls objectAtIndex:0];
    STAssertEqualObjects(firstUrl,
                         [bundleUrl URLByAppendingPathComponent:@"01 - Lost in the Echo.flac"],
                         nil);
    
    NSURL* lastUrl = [fileUrls objectAtIndex:12];
    STAssertEqualObjects(lastUrl, 
                         [bundleUrl URLByAppendingPathComponent:@"13 - What I've Done (Live).flac"],
                         nil);
}

@end
