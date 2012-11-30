//
//  ORGMPluginManager.h
//  OrigamiEngine
//
//  Created by ap4y on 8/9/12.
//
//

#import <Foundation/Foundation.h>
#import "ORGMCommonProtocols.h"

/**
 The `ORGMPluginManager` provides transparent access to the supported decoders, sources and containers. All supported plugins are conform to the corresponding protocol, thus you have common protocol among same type plugins.
 */
@interface ORGMPluginManager : NSObject
+ (ORGMPluginManager*)sharedManager;

/**
 Factory method for source plugins.
 
 @discussion Source class is determined from the url scheme.
 
 @param url A url objects used to determine corresponding source.
 @param error A pointer to an `NSError` object. You do not need to create an `NSError` object.
 
 @return A new source object or `nil` if corresponding plugin is not found.
 */
- (id<ORGMSource>)sourceForURL:(NSURL*)url error:(NSError **)error;

/**
 Factory method for decoder plugins.
 
 @discussion Decoder class is determined from the source url extension.
 
 @param source A source object used to determine corresponding decoder.
 @param error A pointer to an `NSError` object. You do not need to create an `NSError` object.
 
 @return A new decoder object or `nil` if corresponding plugin is not found.
 */
- (id<ORGMDecoder>)decoderForSource:(id<ORGMSource>)source error:(NSError **)error;

/**
 Searches for the container decoder plugin and parses container content.
 
 @discussion Decoder class is determined from the url extension.
 
 @param url A url objects used to determine corresponding container decoder.
 @param error A pointer to an `NSError` object. You do not need to create an `NSError` object.
 
 @return An array with track urls from the container or `nil` if corresponding plugin is not found.
 */
- (NSArray*)urlsForContainerURL:(NSURL*)url error:(NSError **)error;
@end
