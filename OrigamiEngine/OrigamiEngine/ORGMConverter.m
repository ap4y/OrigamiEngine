//
//  ORGMConverter.m
//  Origami-engine
//
//  Created by ap4y on 8/5/12.
//
//

#import "ORGMConverter.h"

#import "ORGMInputUnit.h"
#import "ORGMOutputUnit.h"

@interface ORGMConverter () {
    AudioStreamBasicDescription _inputFormat;
    AudioStreamBasicDescription _outputFormat;
    AudioConverterRef _converter;
    
    void *callbackBuffer;
    void *writeBuf;
}

@property (nonatomic, unsafe_unretained) ORGMInputUnit* inputUnit;
@property (nonatomic, unsafe_unretained) ORGMOutputUnit* outputUnit;
@property (nonatomic, strong) NSMutableData* convertedData;

@end

@implementation ORGMConverter

- (id)initWithInputFormat:(ORGMInputUnit*)inputUnit {
    self = [super init];
    if (self) {
        self.convertedData = [[NSMutableData alloc] init];

        self.inputUnit = inputUnit;
        _inputFormat = inputUnit.format;
        
        writeBuf = malloc(CHUNK_SIZE);        
    }
    return self;
}

- (void)dealloc {
    free(callbackBuffer);
    free(writeBuf);
    [super dealloc];
}

#pragma mark - public

- (BOOL)setupWithOutputUnit:(ORGMOutputUnit*)outputUnit {
    self.outputUnit = outputUnit;
    _outputFormat = outputUnit.format;
    callbackBuffer =
    malloc((CHUNK_SIZE/_outputFormat.mBytesPerFrame)*_inputFormat.mBytesPerPacket);
    
    OSStatus stat = AudioConverterNew(&_inputFormat, &_outputFormat, &_converter);
    if (stat != noErr) {
        NSLog(NSLocalizedString(@"Error creating converter", nil));
        return NO;
    }
    
    if (_inputFormat.mChannelsPerFrame == 1)
    {
        SInt32 channelMap[2] = { 0, 0 };
        
        stat = AudioConverterSetProperty(_converter,
                                         kAudioConverterChannelMap,
                                         sizeof(channelMap),
                                         channelMap);
        if (stat != noErr) {
            NSLog(NSLocalizedString(@"Error mapping channels", nil));
            return NO;
        }
    }
    
    return YES;
}

- (void)process {
    int amountConverted = 0;
    do {
        amountConverted = [self convert:writeBuf amount:CHUNK_SIZE];        
        dispatch_sync([ORGMQueues lock_queue], ^{
            [_convertedData appendBytes:writeBuf length:amountConverted];
        });
    } while (amountConverted > 0 && _convertedData.length < BUFFER_SIZE);
    
    if (_convertedData.length >= BUFFER_SIZE && !_outputUnit.isProcessing) {
        [_outputUnit process];
    }
}

#pragma mark - private

- (int)convert:(void *)dest amount:(int)amount
{
	AudioBufferList ioData;
	UInt32 ioNumberFrames;
	OSStatus err;
	
	ioNumberFrames = amount/_outputFormat.mBytesPerFrame;
	ioData.mBuffers[0].mData = dest;
	ioData.mBuffers[0].mDataByteSize = amount;
	ioData.mBuffers[0].mNumberChannels = _outputFormat.mChannelsPerFrame;
	ioData.mNumberBuffers = 1;
	
	err = AudioConverterFillComplexBuffer(_converter, ACInputProc, self, &ioNumberFrames,
                                          &ioData, NULL);
	int amountRead = ioData.mBuffers[0].mDataByteSize;
	if (err == kAudioConverterErr_InvalidInputSize)	{
		amountRead += [self convert:dest + amountRead amount:amount - amountRead];
	}
	
	return amountRead;
}

static OSStatus ACInputProc(AudioConverterRef inAudioConverter,
                            UInt32* ioNumberDataPackets, AudioBufferList* ioData,
                            AudioStreamPacketDescription** outDataPacketDescription,
                            void* inUserData)
{
	ORGMConverter *converter = (ORGMConverter *)inUserData;
	OSStatus err = noErr;
	int amountToWrite;
	
	amountToWrite = (*ioNumberDataPackets)*(converter->_inputFormat.mBytesPerPacket);
    
    NSMutableData* _data = converter.inputUnit.data;
    if (_data.length < amountToWrite) {
        amountToWrite = _data.length;
    }

    dispatch_sync([ORGMQueues lock_queue], ^{
        memcpy(converter->callbackBuffer, _data.bytes, amountToWrite);
        [_data replaceBytesInRange:NSMakeRange(0, amountToWrite) withBytes:NULL length:0];
    });

	if (amountToWrite == 0)
	{
		ioData->mBuffers[0].mDataByteSize = 0;
		*ioNumberDataPackets = 0;
		
		return 100;
	}
    
	ioData->mBuffers[0].mData = converter->callbackBuffer;
	ioData->mBuffers[0].mDataByteSize = amountToWrite;
	ioData->mBuffers[0].mNumberChannels = (converter->_inputFormat.mChannelsPerFrame);
	ioData->mNumberBuffers = 1;
	
	return err;
}

@end
