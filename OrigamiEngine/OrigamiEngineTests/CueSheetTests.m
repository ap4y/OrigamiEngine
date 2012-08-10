//
//  CueSheetTests.m
//  OrigamiEngine
//
//  Created by ap4y on 8/10/12.
//
//

#import "CueSheetTests.h"
#import "CueSheet.h"

@interface CueSheetTests () {
    NSURL* _multiFileCueUrl;
    NSURL* _singleFileCueUrl;
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
    CueSheet* cueSheet =
        [[CueSheet alloc] initWithURL:_singleFileCueUrl];
    
    STAssertNotNil(cueSheet, nil);
    STAssertEquals(cueSheet.tracks.count, 14U, nil);

    NSURL* bundleUrl = [NSBundle bundleForClass:[self class]].bundleURL;
    NSURL* url = [bundleUrl URLByAppendingPathComponent:@"NEW!.flac"];
    
    CueSheetTrack* firstTrack = [cueSheet.tracks objectAtIndex:0];
    STAssertEqualObjects(firstTrack.title, @"Warning", nil);
    STAssertEqualObjects(firstTrack.artist, @"LAMA", nil);
    STAssertEqualObjects(firstTrack.album, @"NEW!", nil);
    STAssertEqualObjects(firstTrack.track, @"01", nil);
    STAssertEqualObjects(firstTrack.year, @"2011", nil);
    STAssertEqualObjects(firstTrack.genre, @"Rock", nil);
    STAssertEqualsWithAccuracy(firstTrack.time, 0.0, 0.1, nil);
    STAssertEqualObjects(firstTrack.url, url, nil);
    
    CueSheetTrack* lastTrack = [cueSheet.tracks objectAtIndex:13];
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
    CueSheet* cueSheet =
    [[CueSheet alloc] initWithURL:_multiFileCueUrl];
    
    STAssertNotNil(cueSheet, nil);
    STAssertEquals(cueSheet.tracks.count, 12U, nil);
    
    NSURL* bundleUrl = [NSBundle bundleForClass:[self class]].bundleURL;
    
    CueSheetTrack* firstTrack = [cueSheet.tracks objectAtIndex:0];
    STAssertEqualObjects(firstTrack.title, @"Amerigo", nil);
    STAssertEqualObjects(firstTrack.artist, @"Patti Smith", nil);
    STAssertEqualObjects(firstTrack.album, @"Banga", nil);
    STAssertEqualObjects(firstTrack.track, @"01", nil);
    STAssertEqualObjects(firstTrack.year, @"2012", nil);
    STAssertEqualObjects(firstTrack.genre, @"Rock", nil);
    STAssertEqualsWithAccuracy(firstTrack.time, 0.0, 0.1, nil);
    STAssertEqualObjects([bundleUrl URLByAppendingPathComponent:@"01 - Amerigo.wav"],
                         firstTrack.url,
                         nil);
    
    CueSheetTrack* lastTrack = [cueSheet.tracks objectAtIndex:11];
    STAssertEqualObjects(lastTrack.title, @"After The Gold Rush", nil);
    STAssertEqualObjects(lastTrack.artist, @"Patti Smith", nil);
    STAssertEqualObjects(lastTrack.album, @"Banga", nil);
    STAssertEqualObjects(lastTrack.track, @"12", nil);
    STAssertEqualObjects(lastTrack.year, @"2012", nil);
    STAssertEqualObjects(lastTrack.genre, @"Rock", nil);
    STAssertEqualsWithAccuracy(lastTrack.time, 0.0, 0.1, nil);
    STAssertEqualObjects([bundleUrl URLByAppendingPathComponent:@"12 - After The Gold Rush.wav"],
                         lastTrack.url,
                         nil);
}

@end
