#import <AudioToolbox/AudioToolbox.h>
#import <AudioUnit/AudioUnit.h>

@class OutputNode;

@interface OutputCoreAudio : NSObject {
	id outputController;
	
	AudioUnit outputUnit;
	AURenderCallbackStruct renderCallback;	
	AudioStreamBasicDescription deviceFormat;
}

- (id)initWithController:(id)c;

- (BOOL)setup;
- (void)start;
- (void)pause;
- (void)resume;
- (void)stop;

- (void)setVolume:(double) v;

@end
