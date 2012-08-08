@class ORGMOutputUnit;
@interface ORGMCoreAudioOutput : NSObject
- (id)initWithController:(ORGMOutputUnit*)unit;

- (BOOL)setup;
- (void)start;
- (void)pause;
- (void)resume;
- (void)stop;

@end
