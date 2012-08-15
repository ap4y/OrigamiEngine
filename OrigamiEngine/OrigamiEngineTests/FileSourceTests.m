//
//  FileSourceTests.m
//  OrigamiEngine
//
//  Created by ap4y on 8/15/12.
//
//

#import "FileSourceTests.h"

#import "FileSource.h"

@interface FileSourceTests ()
@property (retain, nonatomic) FileSource* source;
@end

@implementation FileSourceTests

- (void)setUp {
    [super setUp];
    
    _source = [[FileSource alloc] init];
    NSURL* flacUrl = [[NSBundle bundleForClass:self.class] URLForResource:@"multiple-vc"
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
    NSURL* flacUrl = [[NSBundle bundleForClass:self.class] URLForResource:@"multiple-vc"
                                                            withExtension:@"flac"];
    STAssertEqualObjects([_source url], flacUrl, nil);
}

- (void)testFileSourceShouldReturnCorrectSize {
    STAssertEquals(4754L, [_source size], nil);
}

- (void)testFileSourceShouldSeekToPositionWithWhenceAndTellCurrentPosition {
    STAssertTrue([_source seek:100 whence:SEEK_SET], nil);
    STAssertEquals(100L, [_source tell], nil);
    STAssertTrue([_source seek:100 whence:SEEK_CUR], nil);
    STAssertEquals(200L, [_source tell], nil);
    STAssertTrue([_source seek:0 whence:SEEK_END], nil);
    STAssertEquals(4754L, [_source tell], nil);
}

- (void)testFileSourceShouldReadData {
    void* buffer = malloc(100);
    STAssertEquals([_source read:buffer amount:100], 100, nil);
    free(buffer);
}

@end
