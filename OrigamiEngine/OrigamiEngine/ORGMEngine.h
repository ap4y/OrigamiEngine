//
//  ORGMEngine.h
//  Origami-engine
//
//  Created by ap4y on 8/5/12.
//
//

#import <Foundation/Foundation.h>

typedef enum : NSInteger {
    ORGMEngineStateStopped,
    ORGMEngineStatePlaying,
    ORGMEngineStatePaused    
} ORGMEngineState;

@interface ORGMEngine : NSObject
@property (assign, nonatomic, readonly) ORGMEngineState currentState;

- (void)playUrl:(NSURL*)url;
- (void)pause;
- (void)resume;
- (void)stop;

- (double)trackTime;
- (double)amountPlayed;
@end
