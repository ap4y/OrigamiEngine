#import <AudioToolbox/AudioToolbox.h>
#import <AudioUnit/AudioUnit.h>

#import "ORGMCoreAudioOutput.h"
#import "ORGMOutputUnit.h"

@interface ORGMCoreAudioOutput () {
	AudioUnit outputUnit;
	AURenderCallbackStruct renderCallback;
	AudioStreamBasicDescription deviceFormat;
}
@property (retain, nonatomic) ORGMOutputUnit* outUnit;
@end

@implementation ORGMCoreAudioOutput
@synthesize outUnit;

#pragma mark - public
- (id)initWithController:(ORGMOutputUnit*)unit {
	self = [super init];
	if (self)
	{
		self.outUnit = unit;
		outputUnit = NULL;
	}
	
	return self;
}

- (void)dealloc {
    [outUnit release];
	[self stop];
	[super dealloc];
}

- (void)start {
	AudioOutputUnitStart(outputUnit);
}

- (void)pause {
	AudioOutputUnitStop(outputUnit);
}

- (void)resume {
    AudioOutputUnitStart(outputUnit);
}

- (void)stop {
	if (outputUnit) {
        AudioOutputUnitStop(outputUnit);
		AudioUnitUninitialize (outputUnit);
		outputUnit = NULL;
	}
}

- (BOOL)setup {
	if (outputUnit) {
		[self stop];
    }
	
	AudioComponentDescription desc;
	OSStatus err;
	
	desc.componentType = kAudioUnitType_Output;
	desc.componentSubType = kAudioUnitSubType_RemoteIO;
	desc.componentManufacturer = kAudioUnitManufacturer_Apple;
	desc.componentFlags = 0;
	desc.componentFlagsMask = 0;
		
    AudioComponent comp;
	if ((comp = AudioComponentFindNext(NULL, &desc)) == NULL) {
		return NO;
    }
	
	if (AudioComponentInstanceNew(comp, &outputUnit)) {
		return NO;
    }
	
	if (AudioUnitInitialize(outputUnit) != noErr)
		return NO;
	
	UInt32 size = sizeof(AudioStreamBasicDescription);
	Boolean outWritable;
	AudioUnitGetPropertyInfo(outputUnit,
							 kAudioUnitProperty_StreamFormat,
							 kAudioUnitScope_Output,
							 0,
							 &size,
							 &outWritable);

	err = AudioUnitGetProperty (outputUnit,
								kAudioUnitProperty_StreamFormat,
								kAudioUnitScope_Output,
								0,
								&deviceFormat,
								&size);
	
	if (err != noErr)
		return NO;
	
	deviceFormat.mChannelsPerFrame = 2;
	deviceFormat.mFormatFlags &= ~kLinearPCMFormatFlagIsNonInterleaved;
    deviceFormat.mFormatFlags &= ~kLinearPCMFormatFlagIsFloat;
    deviceFormat.mFormatFlags = kLinearPCMFormatFlagIsSignedInteger;
	deviceFormat.mBytesPerFrame =
        deviceFormat.mChannelsPerFrame*(deviceFormat.mBitsPerChannel/8);
	deviceFormat.mBytesPerPacket =
        deviceFormat.mBytesPerFrame * deviceFormat.mFramesPerPacket;
	
	AudioUnitSetProperty (outputUnit,
                          kAudioUnitProperty_StreamFormat,
                          kAudioUnitScope_Output,
                          0,
                          &deviceFormat,
                          size);
	AudioUnitSetProperty (outputUnit,
                          kAudioUnitProperty_StreamFormat,
                          kAudioUnitScope_Input,
                          0,
                          &deviceFormat,
                          size);
	
	renderCallback.inputProc = Sound_Renderer;
	renderCallback.inputProcRefCon = self;
	
	AudioUnitSetProperty(outputUnit, kAudioUnitProperty_SetRenderCallback,
                         kAudioUnitScope_Input, 0, &renderCallback,
                         sizeof(AURenderCallbackStruct));
	
	[outUnit setFormat:&deviceFormat];
	return YES;
}

#pragma mark - private

static OSStatus Sound_Renderer(void *inRefCon,
                               AudioUnitRenderActionFlags *ioActionFlags,
                               const AudioTimeStamp  *inTimeStamp,
                               UInt32 inBusNumber,
                               UInt32 inNumberFrames,
                               AudioBufferList  *ioData) {
	ORGMCoreAudioOutput *output = (ORGMCoreAudioOutput *)inRefCon;
	OSStatus err = noErr;
	void *readPointer = ioData->mBuffers[0].mData;
	
	int amountToRead, amountRead;
	
	amountToRead = inNumberFrames*(output->deviceFormat.mBytesPerPacket);
	amountRead = [output->outUnit readData:(readPointer) amount:amountToRead];

	if (amountRead < amountToRead) {
		int amountRead2;
		amountRead2 = [output->outUnit readData:(readPointer+amountRead) amount:amountToRead-amountRead];
		amountRead += amountRead2;
	}
	
	ioData->mBuffers[0].mDataByteSize = amountRead;
	ioData->mBuffers[0].mNumberChannels = output->deviceFormat.mChannelsPerFrame;
	ioData->mNumberBuffers = 1;
	
	return err;
}

@end
