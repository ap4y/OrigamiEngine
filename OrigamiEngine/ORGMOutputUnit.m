//
// ORGMOutputUnit.m
//
// Copyright (c) 2012 ap4y (lod@pisem.net)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#import "ORGMOutputUnit.h"

#import "ORGMInputUnit.h"

@interface ORGMOutputUnit () {
    AudioUnit outputUnit;
    AURenderCallbackStruct renderCallback;

    AudioStreamBasicDescription _format;
    unsigned long long _amountPlayed;
}
@property (strong, nonatomic) ORGMConverter *converter;

- (int)readData:(void *)ptr amount:(int)amount;
@end

@implementation ORGMOutputUnit

- (id)initWithConverter:(ORGMConverter *)converter {
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
    _isProcessing  = NO;
    self.converter = nil;
    if (outputUnit) {
        AudioOutputUnitStop(outputUnit);
        AudioUnitUninitialize(outputUnit);
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

- (void)setVolume:(float)volume {
    AudioUnitSetParameter(outputUnit, kHALOutputParam_Volume, kAudioUnitScope_Global, 0, volume * 0.01f, 0);
}

- (void)setSampleRate:(double)sampleRate {
    UInt32 size = sizeof(AudioStreamBasicDescription);
    _format.mSampleRate = sampleRate;
    AudioUnitSetProperty(outputUnit,
                         kAudioUnitProperty_StreamFormat,
                         kAudioUnitScope_Output,
                         0,
                         &_format,
                         size);

    AudioUnitSetProperty(outputUnit,
                         kAudioUnitProperty_StreamFormat,
                         kAudioUnitScope_Input,
                         0,
                         &_format,
                         size);
    [self setFormat:&_format];
}

#pragma mark - callbacks

static OSStatus Sound_Renderer(void *inRefCon,
                               AudioUnitRenderActionFlags *ioActionFlags,
                               const AudioTimeStamp  *inTimeStamp,
                               UInt32 inBusNumber,
                               UInt32 inNumberFrames,
                               AudioBufferList  *ioData) {
    ORGMOutputUnit *output = (__bridge ORGMOutputUnit *)inRefCon;
    OSStatus err = noErr;
    void *readPointer = ioData->mBuffers[0].mData;

    int amountToRead, amountRead;

    amountToRead = inNumberFrames * (output->_format.mBytesPerPacket);
    amountRead = [output readData:(readPointer) amount:amountToRead];

    if (amountRead < amountToRead) {
        int amountRead2;
        amountRead2 = [output readData:(readPointer+amountRead) amount:amountToRead-amountRead];
        amountRead += amountRead2;
    }

    ioData->mBuffers[0].mDataByteSize = amountRead;
    ioData->mBuffers[0].mNumberChannels = output->_format.mChannelsPerFrame;
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
#if __IPHONE_OS_VERSION_MIN_REQUIRED
    desc.componentSubType = kAudioUnitSubType_RemoteIO;
#else
    desc.componentSubType = kAudioUnitSubType_DefaultOutput;
#endif
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

    AudioStreamBasicDescription deviceFormat;
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
    deviceFormat.mBytesPerFrame = deviceFormat.mChannelsPerFrame*(deviceFormat.mBitsPerChannel/8);
    deviceFormat.mBytesPerPacket = deviceFormat.mBytesPerFrame * deviceFormat.mFramesPerPacket;

    if (_outputFormat == ORGMOutputFormat24bit) {
        deviceFormat.mBytesPerFrame = 6;
        deviceFormat.mBytesPerPacket = 6;
        deviceFormat.mBitsPerChannel = 24;
    }

    AudioUnitSetProperty(outputUnit,
            kAudioUnitProperty_StreamFormat,
            kAudioUnitScope_Output,
            0,
            &deviceFormat,
            size);
    AudioUnitSetProperty(outputUnit,
            kAudioUnitProperty_StreamFormat,
            kAudioUnitScope_Input,
            0,
            &deviceFormat,
            size);

    renderCallback.inputProc = Sound_Renderer;
    renderCallback.inputProcRefCon = (__bridge void * _Nullable)(self);

    AudioUnitSetProperty(outputUnit, kAudioUnitProperty_SetRenderCallback,
            kAudioUnitScope_Input, 0, &renderCallback,
            sizeof(AURenderCallbackStruct));

    [self setFormat:&deviceFormat];
    return YES;
}

- (int)readData:(void *)ptr amount:(int)amount {
    if (!_converter) {
        @throw [NSException exceptionWithName:NSInternalInconsistencyException
                                       reason:NSLocalizedString(@"Converter is undefined", nil)
                                     userInfo:nil];
    }
    int bytesRead = [_converter shiftBytes:amount buffer:ptr];
    _amountPlayed += bytesRead;

    if ([_converter isReadyForBuffering]) {
        dispatch_source_merge_data([ORGMQueues buffering_source], 1);
    }

    return bytesRead;
}

- (void)setFormat:(AudioStreamBasicDescription *)f {
    _format = *f;
}

@end
