//
//  PluginManagerTests.m
//  OrigamiEngine
//
//  Created by ap4y on 8/15/12.
//
//

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
    NSURL* url = [NSURL URLWithString:@"http2://mp3.com/test.mp3"];
    NSError *error;
    STAssertNil([[ORGMPluginManager sharedManager] sourceForURL:url error:&error], nil);
    STAssertEquals(error.code, ORGMEngineErrorCodesSourceFailed, nil);
}

- (void)testManagerShouldReturnErrorForUnknownDecoder {
    NSURL* url = [NSURL URLWithString:@"file:///User/test.mp34"];
    id<ORGMSource> source = [[ORGMPluginManager sharedManager] sourceForURL:url error:nil];
    [source open:url];
    NSError *error;
    STAssertNil([[ORGMPluginManager sharedManager] decoderForSource:source error:&error], nil);
    STAssertEquals(error.code, ORGMEngineErrorCodesDecoderFailed, nil);
}

- (void)testManagerShouldReturnErrorForUnknownContainer {
    NSURL* url = [NSURL URLWithString:@"file:///User/test.mp34"];
    NSError *error;
    STAssertNil([[ORGMPluginManager sharedManager] urlsForContainerURL:url error:&error], nil);
    STAssertEquals(error.code, ORGMEngineErrorCodesContainerFailed, nil);
}

- (void)testManagerShouldReturnSourceForHTTPScheme {
    NSURL* url = [NSURL URLWithString:@"http://mp3.com/test.mp3"];
    id<ORGMSource> source = [[ORGMPluginManager sharedManager] sourceForURL:url error:nil];
    STAssertEqualObjects([source class], [HTTPSource class], nil);
}

- (void)testManagerShouldReturnSourceForFileScheme {
    NSURL* url = [NSURL URLWithString:@"file://test.mp3"];
    id<ORGMSource> source = [[ORGMPluginManager sharedManager] sourceForURL:url error:nil];
    STAssertEqualObjects([source class], [FileSource class], nil);
}

- (void)testManagerShouldReturnDecoderForMp3Extension {
    NSURL* url = [NSURL URLWithString:@"file:///User/test.mp3"];
    id<ORGMSource> source = [[ORGMPluginManager sharedManager] sourceForURL:url error:nil];
    [source open:url];
    id<ORGMDecoder> decoder = [[ORGMPluginManager sharedManager] decoderForSource:source error:nil];
    STAssertEqualObjects([decoder class], [CoreAudioDecoder class], nil);
}

- (void)testManagerShouldReturnDecoderForFlacExtension {
    NSURL* url = [NSURL URLWithString:@"file:///User/test.flac"];
    id<ORGMSource> source = [[ORGMPluginManager sharedManager] sourceForURL:url error:nil];
    [source open:url];
    id<ORGMDecoder> decoder = [[ORGMPluginManager sharedManager] decoderForSource:source error:nil];
    STAssertEqualObjects([decoder class], [FlacDecoder class], nil);
}

- (void)testManagerShouldReturnDecoderForCueExtension {
    NSURL* url = [NSURL URLWithString:@"file:///User/test.cue"];
    id<ORGMSource> source = [[ORGMPluginManager sharedManager] sourceForURL:url error:nil];
    [source open:url];
    id<ORGMDecoder> decoder = [[ORGMPluginManager sharedManager] decoderForSource:source error:nil];
    STAssertEqualObjects([decoder class], [CueSheetDecoder class], nil);
}

- (void)testManagerShouldReturnUrlsFromCueFile {
    NSURL* url = [[NSBundle bundleForClass:[self class]] URLForResource:@"multi_file"
                                                          withExtension:@"cue"];

    NSArray* urls = [[ORGMPluginManager sharedManager] urlsForContainerURL:url error:nil];
    STAssertEquals(urls.count, 12U, nil);
}

- (void)testManagerShouldReturnUrlsFromM3uFile {
    NSURL* url = [[NSBundle bundleForClass:[self class]] URLForResource:@"test"
                                                          withExtension:@"m3u"];
    
    NSArray* urls = [[ORGMPluginManager sharedManager] urlsForContainerURL:url error:nil];
    STAssertEquals(urls.count, 13U, nil);
}

@end
