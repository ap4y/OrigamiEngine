//
//  ORGMEngine.m
//  Origami-engine
//
//  Created by ap4y on 8/5/12.
//
//

#import "ORGMEngine.h"

#import "ORGMInputUnit.h"
#import "ORGMOutputUnit.h"
#import "ORGMConverter.h"

@interface ORGMEngine ()
@property (retain, nonatomic) ORGMInputUnit* input;
@property (retain, nonatomic) ORGMOutputUnit* output;
@property (retain, nonatomic) ORGMConverter* converter;
@property (assign, nonatomic) ORGMEngineState currentState;
@end

@implementation ORGMEngine

- (id)init {
    self = [super init];
    if (self) {
        [self setup];
        [self setCurrentState:ORGMEngineStateStopped];
        [self addObserver:self forKeyPath:@"currentState"
                  options:NSKeyValueObservingOptionNew
                  context:nil];
    }
    return self;
}

- (void)dealloc {
    [_input release];
    [_output release];
    [_converter release];
    [super dealloc];
}

#pragma mark - public

- (void)playUrl:(NSURL*)url {
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT , 0), ^{
        [self setupNewInput:url];
        self.converter = [[ORGMConverter alloc] initWithInputFormat:_input];
        self.output = [[ORGMOutputUnit alloc] initWithConverter:_converter];
        
        if (![_converter setupWithOutputUnit:_output]) {
            @throw [NSException exceptionWithName:NSInternalInconsistencyException
                                           reason:NSLocalizedString(@"Couldn't setup converter...", nil)
                                         userInfo:nil];
        }
        
        [self setCurrentState:ORGMEngineStatePlaying];
        dispatch_source_merge_data([ORGMQueues buffering_source], 1);
    });
}

- (void)pause {
    [_output pause];
    [self setCurrentState:ORGMEngineStatePaused];
}

- (void)resume {
    [_output resume];
    [self setCurrentState:ORGMEngineStatePlaying];
}

- (void)stop {
    [_output stop];
    self.output = nil;
    [_input close];
    self.input = nil;
    self.converter = nil;
    [self setCurrentState:ORGMEngineStateStopped];
}

- (double)trackTime {
	return [_output framesToSeconds:_input.framesCount];
}

- (double)amountPlayed {
	return [_output amountPlayed];
}

- (NSDictionary *)metadata {
    return [_input metadata];
}

- (void)seekToTime:(double)time {
    [_output seek:time];
    [_input seek:time];
}
#pragma mark - private
- (void)setupNewInput:(NSURL*)url {
    self.input = [[ORGMInputUnit alloc] init];
    if (![_input openWithUrl:url]) {
        @throw [NSException exceptionWithName:NSInternalInconsistencyException
                                       reason:NSLocalizedString(@"Couldn't open source...", nil)
                                     userInfo:nil];
    }
    [_input addObserver:self forKeyPath:@"endOfInput"
                options:NSKeyValueObservingOptionNew
                context:nil];
}

- (void)setNextUrl:(NSURL*)url {
    if (!url) {
        [self stop];
    } else {
        dispatch_async([ORGMQueues processing_queue], ^{
            [self setupNewInput:url];
            [_converter reinitWithNewInput:_input];
            [_output seek:0.0]; //to reset amount played
            [self setCurrentState:ORGMEngineStatePlaying]; //trigger delegate method
        });
    }
}

- (void)observeValueForKeyPath:(NSString *)keyPath
                      ofObject:(id)object
                        change:(NSDictionary *)change
                       context:(void *)context {
    if (!_delegate)
        return;
    
    if ([keyPath isEqualToString:@"currentState"]) {
        dispatch_async(dispatch_get_main_queue(), ^{
            [_delegate engine:self didChangeState:_currentState];
        });
    } else if ([keyPath isEqualToString:@"endOfInput"]) {
        dispatch_async(dispatch_get_main_queue(), ^{
            [self setNextUrl:[_delegate engineIsExpectNextUrl:self]];            
        });
    }
}

- (void)setup {
    dispatch_source_set_event_handler([ORGMQueues buffering_source], ^{
        [_input process];
        [_converter process];
    });
    
    dispatch_resume([ORGMQueues buffering_source]);
}

@end
