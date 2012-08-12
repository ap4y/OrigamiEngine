//
//  ORGMPluginManager.m
//  OrigamiEngine
//
//  Created by ap4y on 8/9/12.
//
//

#import "ORGMPluginManager.h"

#import "HTTPSource.h"
#import "FileSource.h"

#import "FlacDecoder.h"
#import "CoreAudioDecoder.h"
#import "CueSheetDecoder.h"

#import "CueSheetContainer.h"
#import "M3uContainer.h"

@interface ORGMPluginManager ()
@property(retain, nonatomic) NSDictionary* sources;
@property(retain, nonatomic) NSDictionary* decoders;
@property(retain, nonatomic) NSDictionary* containers;
@end

@implementation ORGMPluginManager

+ (ORGMPluginManager*)sharedManager {
    static ORGMPluginManager* _sharedManager;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _sharedManager = [[ORGMPluginManager alloc] init];
    });
    
    return _sharedManager;
}

- (id)init {
    self = [super init];
    if (self) {
        
        /* Sources */
        self.sources = [NSDictionary dictionaryWithObjectsAndKeys:
                        [HTTPSource class], [HTTPSource scheme],
                        [FileSource class], [FileSource scheme],
                        nil];
                 
        /* Decoders */
        NSMutableDictionary* decodersDict = [NSMutableDictionary dictionary];
        [[FlacDecoder fileTypes] enumerateObjectsUsingBlock:^(id obj, NSUInteger idx,
                                                              BOOL *stop) {
            [decodersDict setObject:[FlacDecoder class] forKey:obj];
        }];
        [[CoreAudioDecoder fileTypes] enumerateObjectsUsingBlock:^(id obj, NSUInteger idx,
                                                                   BOOL *stop) {
            [decodersDict setObject:[CoreAudioDecoder class] forKey:obj];
        }];
        [[CueSheetDecoder fileTypes] enumerateObjectsUsingBlock:^(id obj, NSUInteger idx,
                                                                  BOOL *stop) {
            [decodersDict setObject:[CueSheetDecoder class] forKey:obj];
        }];
        self.decoders = decodersDict;
        
        /* Containers */        
        NSMutableDictionary* containersDict = [NSMutableDictionary dictionary];
        [[CueSheetContainer fileTypes] enumerateObjectsUsingBlock:^(id obj,
                                                                    NSUInteger idx,
                                                                    BOOL *stop) {
            [containersDict setObject:[CueSheetContainer class] forKey:obj];
        }];
        [[M3uContainer fileTypes] enumerateObjectsUsingBlock:^(id obj,
                                                               NSUInteger idx,
                                                               BOOL *stop) {
            [containersDict setObject:[M3uContainer class] forKey:obj];
        }];
        
        self.containers = containersDict;
    }
    return self;
}

- (void)dealloc {
    [_sources release];
    [_decoders release];
    [_containers release];
    [super dealloc];
}

- (id<ORGMSource>)sourceForURL:(NSURL*)url {
	NSString *scheme = [url scheme];	
	Class source = [_sources objectForKey:scheme];
	if (!source) {
        @throw [NSException exceptionWithName:NSInvalidArgumentException
                                       reason:NSLocalizedString(@"Unable to find source", nil)
                                     userInfo:nil];
    }
	return [[[source alloc] init] autorelease];
}

- (id<ORGMDecoder>)decoderForSource:(id<ORGMSource>)source {
    if (!source || ![source url]) {
        return nil;
    }
	NSString* extension = [[[source url] path] pathExtension];
	Class decoder = [_decoders objectForKey:[extension lowercaseString]];
	if (!decoder) {
		@throw [NSException exceptionWithName:NSInvalidArgumentException
                                       reason:NSLocalizedString(@"Unable to find decoder", nil)
                                     userInfo:nil];
	}
    
	return [[[decoder alloc] init] autorelease];
}

- (NSArray*)urlsForContainerURL:(NSURL*)url {
	NSString *ext = [[url path] pathExtension];	
	Class container = [_containers objectForKey:[ext lowercaseString]];
	if (!container) {
		@throw [NSException exceptionWithName:NSInvalidArgumentException
                                       reason:NSLocalizedString(@"Unable to find container", nil)
                                     userInfo:nil];
	}
    
	return [container urlsForContainerURL:url];
}
@end
