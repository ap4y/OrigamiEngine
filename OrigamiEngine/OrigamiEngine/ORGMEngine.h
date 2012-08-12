//
//  ORGMEngine.h
//  Origami-engine
//
//  Created by ap4y on 8/5/12.
//
//

#import <Foundation/Foundation.h>
 
@protocol ORGMEngineDelegate;
typedef enum : NSInteger {
    ORGMEngineStateStopped,
    ORGMEngineStatePlaying,
    ORGMEngineStatePaused    
} ORGMEngineState;

@interface ORGMEngine : NSObject
@property (assign, nonatomic, readonly) ORGMEngineState currentState;
@property (unsafe_unretained, nonatomic) id<ORGMEngineDelegate> delegate;

- (void)playUrl:(NSURL*)url;
- (void)pause;
- (void)resume;
- (void)stop;

- (double)trackTime;
- (double)amountPlayed;
- (NSDictionary*)metadata;

- (void)seekToTime:(double)time;
@end

@protocol ORGMEngineDelegate <NSObject>
- (NSURL*)engineIsExpectNextUrl:(ORGMEngine*)engine;
@optional
- (void)engine:(ORGMEngine*)engine didChangeState:(ORGMEngineState)state;
@end
