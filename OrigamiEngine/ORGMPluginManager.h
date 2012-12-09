//
// ORGMPluginManager.h
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

#import <Foundation/Foundation.h>
#import "ORGMCommonProtocols.h"

/**
 The `ORGMPluginManager` provides transparent access to the supported decoders, sources and containers. All supported plugins are conform to the corresponding protocol, thus you have common protocol among same type plugins.
 */
@interface ORGMPluginManager : NSObject
+ (ORGMPluginManager *)sharedManager;

/**
 Factory method for source plugins.
 
 @discussion Source class is determined from the url scheme.
 
 @param url A url objects used to determine corresponding source.
 @param error A pointer to an `NSError` object. You do not need to create an `NSError` object.
 
 @return A new source object or `nil` if corresponding plugin is not found.
 */
- (id<ORGMSource>)sourceForURL:(NSURL *)url error:(NSError **)error;

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
- (NSArray *)urlsForContainerURL:(NSURL *)url error:(NSError **)error;
@end
