//
//  ORGMOutputUnit.m
//  Origami-engine
//
//  Created by ap4y on 8/5/12.
//
//

#import "ORGMOutputUnit.h"

#import "ORGMInputUnit.h"

@interface ORGMOutputUnit () {
    AudioUnit outputUnit;
	AURenderCallbackStruct renderCallback;
	AudioStreamBasicDescription deviceFormat;
    
    AudioStreamBasicDescription _format;
    unsigned long long _amountPlayed;
}
@property (retain, nonatomic) ORGMConverter* converter;

- (int)readData:(void *)ptr amount:(int)amount;
@end

@implementation ORGMOutputUnit

- (id)initWithConverter:(ORGMConverter*)converter {
    self = [super init];
    if (self) {
        outputUnit = NULL;
        [self setup];
        
        self.converter = converter;
        _amountPlayed = 0;
    }
    return self;
}

- (void)dealloc {
    [self stop];
    [super dealloc];
}

#pragma mark - public

- (AudioStreamBasicDescription)format {
    return _format;
}

- (void)process {
    _isProcessing = YES;
    dispatch_async(dispatch_get_main_queue(), ^{
        AudioOutputUnitStart(outputUnit);
    });
}

- (void)pause {
    AudioOutputUnitStop(outputUnit);
}

- (void)resume {
    AudioOutputUnitStart(outputUnit);
}

- (void)stop {
    _isProcessing = NO;
    if (outputUnit) {
        AudioOutputUnitStop(outputUnit);
		AudioUnitUninitialize (outputUnit);
		outputUnit = NULL;
	}
}

- (double)framesToSeconds:(double)framesCount {
	return (framesCount/_format.mSampleRate);
}

- (double)amountPlayed {
	return (_amountPlayed/_format.mBytesPerFrame)/(_format.mSampleRate);
}

- (void)seek:(double)time {
	_amountPlayed = time*_format.mBytesPerFrame*(_format.mSampleRate);
}

#pragma mark - callbacks
static OSStatus Sound_Renderer(void *inRefCon,
                               AudioUnitRenderActionFlags *ioActionFlags,
                               const AudioTimeStamp  *inTimeStamp,
                               UInt32 inBusNumber,
                               UInt32 inNumberFrames,
                               AudioBufferList  *ioData) {
	ORGMOutputUnit *output = (ORGMOutputUnit *)inRefCon;
	OSStatus err = noErr;
	void *readPointer = ioData->mBuffers[0].mData;
	
	int amountToRead, amountRead;
	
	amountToRead = inNumberFrames*(output->deviceFormat.mBytesPerPacket);
	amountRead = [output readData:(readPointer) amount:amountToRead];
    
	if (amountRead < amountToRead) {
		int amountRead2;
		amountRead2 = [output readData:(readPointer+amountRead) amount:amountToRead-amountRead];
		amountRead += amountRead2;
	}
	
	ioData->mBuffers[0].mDataByteSize = amountRead;
	ioData->mBuffers[0].mNumberChannels = output->deviceFormat.mChannelsPerFrame;
	ioData->mNumberBuffers = 1;
	
	return err;
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
	
	[self setFormat:&deviceFormat];
	return YES;
}

- (int)readData:(void *)ptr amount:(int)amount {
    if (!_converter) {
        @throw [NSException exceptionWithName:NSInvalidArchiveOperationException
                                       reason:NSLocalizedString(@"Converter is undefined", nli)
                                     userInfo:nil];
    }
    int n;
    n = amount;
    NSMutableData* convertedData = _converter.convertedData;
    if (convertedData.length < n) {
        n = convertedData.length;
    }
    
    dispatch_sync([ORGMQueues lock_queue], ^{
        memcpy(ptr, convertedData.bytes, n);
        [convertedData replaceBytesInRange:NSMakeRange(0, n) withBytes:NULL length:0];
    });
    _amountPlayed += n;
    
    if (convertedData.length <= 0.5*BUFFER_SIZE && !_converter.inputUnit.isProcessing) {
        [_converter.inputUnit requestNext];
    }
    
    return n;
}

- (void)setFormat:(AudioStreamBasicDescription *)f {
	_format = *f;
}

@end
