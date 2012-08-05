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
@property (nonatomic, strong) ORGMInputUnit* input;
@property (nonatomic, strong) ORGMOutputUnit* output;
@property (nonatomic, strong) ORGMConverter* converter;
@end

@implementation ORGMEngine

- (id)init
{
    self = [super init];
    if (self) {
        [self setup];
    }
    return self;
}

#pragma mark - public

- (void)playUrl:(NSURL*)url {
    self.input = [[ORGMInputUnit alloc] init];
    if (![_input openWithUrl:url]) {
        @throw [NSException exceptionWithName:NSInternalInconsistencyException
                                       reason:NSLocalizedString(@"Couldn't open source...", nil)
                                     userInfo:nil];
	}

    self.converter = [[ORGMConverter alloc] initWithInputFormat:_input];
    self.output = [[ORGMOutputUnit alloc] initWithConverter:_converter];
    
    if (![_converter setupWithOutputUnit:_output]) {
        @throw [NSException exceptionWithName:NSInternalInconsistencyException
                                       reason:NSLocalizedString(@"Couldn't setup converter...", nil)
                                     userInfo:nil];
	}
    
    dispatch_source_merge_data([ORGMQueues buffering_source], 1);
}

- (void)pause {
    [_output pause];
}

- (void)resume {
    [_output resume];
}

- (double)trackTime {
	return [_output framesToSeconds:_input.framesCount];
}

- (double)amountPlayed {
	return [_output amountPlayed];
}
#pragma mark - private

- (void)setup {
    dispatch_source_set_event_handler([ORGMQueues buffering_source], ^{
        [_input process];
        [_converter process];
    });
    
    dispatch_resume([ORGMQueues buffering_source]);
}

@end
