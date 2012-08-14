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

- (void)testManagerShouldReturnSourceForHTTPScheme {
    NSURL* url = [NSURL URLWithString:@"http://mp3.com/test.mp3"];
    id<ORGMSource> source = [[ORGMPluginManager sharedManager] sourceForURL:url];
    STAssertEqualObjects([HTTPSource class], [source class], nil);
}

- (void)testManagerShouldReturnSourceForFileScheme {
    NSURL* url = [NSURL URLWithString:@"file://test.mp3"];
    id<ORGMSource> source = [[ORGMPluginManager sharedManager] sourceForURL:url];
    STAssertEqualObjects([FileSource class], [source class], nil);
}

- (void)testManagerShouldReturnDecoderForMp3Extension {
    NSURL* url = [NSURL URLWithString:@"file:///User/test.mp3"];
    id<ORGMSource> source = [[ORGMPluginManager sharedManager] sourceForURL:url];
    [source open:url];
    id<ORGMDecoder> decoder = [[ORGMPluginManager sharedManager] decoderForSource:source];
    STAssertEqualObjects([CoreAudioDecoder class], [decoder class], nil);
}

- (void)testManagerShouldReturnDecoderForFlacExtension {
    NSURL* url = [NSURL URLWithString:@"file:///User/test.flac"];
    id<ORGMSource> source = [[ORGMPluginManager sharedManager] sourceForURL:url];
    [source open:url];
    id<ORGMDecoder> decoder = [[ORGMPluginManager sharedManager] decoderForSource:source];
    STAssertEqualObjects([FlacDecoder class], [decoder class], nil);
}

- (void)testManagerShouldReturnDecoderForCueExtension {
    NSURL* url = [NSURL URLWithString:@"file:///User/test.cue"];
    id<ORGMSource> source = [[ORGMPluginManager sharedManager] sourceForURL:url];
    [source open:url];
    id<ORGMDecoder> decoder = [[ORGMPluginManager sharedManager] decoderForSource:source];
    STAssertEqualObjects([CueSheetDecoder class], [decoder class], nil);
}

- (void)testManagerShouldReturnUrlsFromCueFile {
    NSURL* url = [[NSBundle bundleForClass:[self class]] URLForResource:@"multi_file"
                                                          withExtension:@"cue"];

    NSArray* urls = [[ORGMPluginManager sharedManager] urlsForContainerURL:url];
    STAssertEquals(12U, urls.count, nil);
}

- (void)testManagerShouldReturnUrlsFromM3uFile {
    NSURL* url = [[NSBundle bundleForClass:[self class]] URLForResource:@"test"
                                                          withExtension:@"m3u"];
    
    NSArray* urls = [[ORGMPluginManager sharedManager] urlsForContainerURL:url];
    STAssertEquals(13U, urls.count, nil);
}

@end
