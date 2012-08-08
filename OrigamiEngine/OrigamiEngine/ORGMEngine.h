//
//  ORGMEngine.h
//  Origami-engine
//
//  Created by ap4y on 8/5/12.
//
//

#import <Foundation/Foundation.h>

@interface ORGMEngine : NSObject
- (void)playUrl:(NSURL*)url;
- (void)pause;
- (void)resume;
- (void)stop;

- (double)trackTime;
- (double)amountPlayed;
@end
