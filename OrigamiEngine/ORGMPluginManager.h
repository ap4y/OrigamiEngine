//
//  ORGMPluginManager.h
//  OrigamiEngine
//
//  Created by ap4y on 8/9/12.
//
//

#import <Foundation/Foundation.h>
#import "ORGMCommonProtocols.h"

@interface ORGMPluginManager : NSObject
+ (ORGMPluginManager*)sharedManager;

- (id<ORGMSource>)sourceForURL:(NSURL*)url error:(NSError **)error;
- (id<ORGMDecoder>)decoderForSource:(id<ORGMSource>)source error:(NSError **)error;
- (NSArray*)urlsForContainerURL:(NSURL*)url error:(NSError **)error;
@end
