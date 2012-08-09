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

- (id<ORGMSource>)sourceForURL:(NSURL*)url;
- (id<ORGMDecoder>)decoderForSource:(id<ORGMSource>)source;
@end
