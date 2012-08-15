//
//  HTTPSourceTests.m
//  OrigamiEngine
//
//  Created by ap4y on 8/15/12.
//
//

#import "HTTPSourceTests.h"

#import "HTTPSource.h"

@interface HTTPSourceTests ()
@property (retain, nonatomic) HTTPSource* source;
@end

@implementation HTTPSourceTests

- (void)setUp {
    [super setUp];
    
    _source = [[HTTPSource alloc] init];
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
    STAssertEqualObjects([HTTPSource scheme], @"http", nil);
}

- (void)testFileSourceShouldReturnValidUrl {
    NSURL* flacUrl = [[NSBundle bundleForClass:self.class] URLForResource:@"multiple-vc"
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
    void* buffer = malloc(100);
    STAssertEquals([_source read:buffer amount:100], 100, nil);
    free(buffer);
}

- (void)testFileSourceShouldCacheData {
    NSArray *paths =
        NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
    NSString* dataPath =
        [[paths objectAtIndex:0] stringByAppendingPathComponent:@"StreamCache"];
    
    NSURL* flacUrl = [[NSBundle bundleForClass:self.class] URLForResource:@"multiple-vc"
                                                            withExtension:@"flac"];
    NSString* fileName = [NSString stringWithFormat:@"%x.%@",
                          [[flacUrl absoluteString] hash],
                          flacUrl.pathExtension];
    NSString* filePath =
        [dataPath stringByAppendingPathComponent:fileName];
    NSFileHandle* fileHandle = [NSFileHandle fileHandleForUpdatingAtPath:filePath];
    NSFileManager* defaultFileManger = [NSFileManager defaultManager];
    
    STAssertTrue([defaultFileManger fileExistsAtPath:filePath], nil);

    [fileHandle seekToEndOfFile];
    STAssertEquals([fileHandle offsetInFile], 4754ULL, nil);
}

@end
