#import <unistd.h>
#import <AudioToolbox/AudioToolbox.h>

#import "CoreAudioDecoder.h"

@interface CoreAudioDecoder () {
    id<ORGMSource>  _source;
    AudioFileID     _audioFile;
	ExtAudioFileRef	_in;
    
	int bitrate;
	int bitsPerSample;
	int channels;
	float frequency;
	long totalFrames;
}

@end

@implementation CoreAudioDecoder

#pragma mark - ORGMEngineObject
+ (NSArray*)fileTypes {
	OSStatus			err;
	UInt32				size;
	NSArray *sAudioExtensions;
	
	size = sizeof(sAudioExtensions);
	err	= AudioFileGetGlobalInfo(kAudioFileGlobalInfo_AllExtensions, 0, NULL,
                                 &size, &sAudioExtensions);
	if(noErr != err) {
		return nil;
	}
	
	return [sAudioExtensions autorelease];
}

+ (NSArray*)mimeTypes {
	return nil;
}

#pragma mark - ORGMDecoder
- (NSDictionary*)properties {
	return [NSDictionary dictionaryWithObjectsAndKeys:
            [NSNumber numberWithInt:channels], @"channels",
            [NSNumber numberWithInt:bitsPerSample], @"bitsPerSample",
            [NSNumber numberWithInt:bitrate], @"bitrate",
            [NSNumber numberWithFloat:frequency], @"sampleRate",
            [NSNumber numberWithLong:totalFrames], @"totalFrames",
            [NSNumber numberWithBool:YES], @"seekable",
            @"big", @"endian",
            nil];
}

- (int)readAudio:(void*)buf frames:(UInt32)frames {
	OSStatus		err;
	AudioBufferList	bufferList;
	UInt32			frameCount;
	
	bufferList.mNumberBuffers			   = 1;
	bufferList.mBuffers[0].mNumberChannels = channels;
	bufferList.mBuffers[0].mData		   = buf;
	bufferList.mBuffers[0].mDataByteSize   = frames * channels * (bitsPerSample/8);
	
	frameCount = frames;
	err		   = ExtAudioFileRead(_in, &frameCount, &bufferList);
	if(err != noErr) {
		return 0;
	}
	
	return frameCount;
}

- (BOOL)open:(id<ORGMSource>)source {
    _source = [source retain];
	OSStatus result = AudioFileOpenWithCallbacks(_source, audioFile_ReadProc, NULL,
                                                 audioFile_GetSizeProc, NULL, 0,
                                                 &_audioFile);
    
    if(noErr != result) {
        return NO;
    }
    
	result = ExtAudioFileWrapAudioFileID(_audioFile, false, &_in);
	if(noErr != result) {
		return NO;
	}
	
	return [self readInfoFromExtAudioFileRef];
}

- (long)seek:(long)frame {
	OSStatus err;
	
	err = ExtAudioFileSeek(_in, frame);
	if(noErr != err) {
		return -1;
	}
	
	return frame;
}

- (void)close {
	OSStatus err;
	    
    err = ExtAudioFileDispose(_in);
	if(noErr != err) {
		NSLog(@"Error closing ExtAudioFile");
	}
    
    err = AudioFileClose(_audioFile);
    if(noErr != err) {
		NSLog(@"Error closing AudioFileID");
	}

    [_source release];
}

#pragma mark - private
- (BOOL)readInfoFromExtAudioFileRef {
	OSStatus					err;
	UInt32						size;
	AudioStreamBasicDescription	asbd;
	
	size = sizeof(asbd);
	err	 = ExtAudioFileGetProperty(_in, kExtAudioFileProperty_FileDataFormat,
                                   &size, &asbd);
	if(err != noErr) {
        ExtAudioFileDispose(_in);
		return NO;
	}
	
    //TODO: add total frame reading for non http data
//	SInt64 total;
//	size = sizeof(total);
//	err	 = ExtAudioFileGetProperty(_in, kExtAudioFileProperty_FileLengthFrames,
//                                   &size, &total);
//	if(err != noErr) {
//		err = ExtAudioFileDispose(_in);
//		return NO;
//	}
//	totalFrames = (long)total;
	
	bitrate	= 0;
	bitsPerSample = asbd.mBitsPerChannel;
	channels	  = asbd.mChannelsPerFrame;
	frequency	  = asbd.mSampleRate;
	
	if(0 == bitsPerSample) {
		bitsPerSample = 16;
	}
	
	AudioStreamBasicDescription	result;
	bzero(&result, sizeof(AudioStreamBasicDescription));
	
	result.mFormatID	= kAudioFormatLinearPCM;
	result.mFormatFlags	= kAudioFormatFlagIsSignedInteger |
                          kLinearPCMFormatFlagIsBigEndian;
	
	result.mSampleRate		 = frequency;
	result.mChannelsPerFrame = channels;
	result.mBitsPerChannel	 = bitsPerSample;
	
	result.mBytesPerPacket	 = channels * (bitsPerSample / 8);
	result.mFramesPerPacket  = 1;
	result.mBytesPerFrame	 = channels * (bitsPerSample / 8);
	
	err = ExtAudioFileSetProperty(_in, kExtAudioFileProperty_ClientDataFormat,
                                  sizeof(result), &result);
	if(noErr != err) {
		err = ExtAudioFileDispose(_in);
		return NO;
	}
		
	return YES;
}

#pragma mark - callback functions
static OSStatus audioFile_ReadProc(void *inClientData,
                                   SInt64 inPosition,
                                   UInt32 requestCount,
                                   void	 *buffer,
                                   UInt32 *actualCount) {
    id<ORGMSource> source = inClientData;
    [source seek:(long)inPosition whence:0];
	*actualCount = [source read:buffer amount:requestCount];
    
	if(requestCount != *actualCount) {
        return -1; //TODO: decide with error code
    }
    
	return noErr;
}

static SInt64 audioFile_GetSizeProc(void *inClientData) {
	id<ORGMSource> source = inClientData;
    SInt64 len = [source size];
	return len;
}

@end
