#include <unistd.h>
#include <AudioToolbox/AudioToolbox.h>

#import "CoreAudioDecoder.h"

@interface CoreAudioDecoder (Private)
- (BOOL) readInfoFromExtAudioFileRef;
@end

@implementation CoreAudioDecoder
static OSStatus 
myAudioFile_ReadProc(void		*inClientData,
					 SInt64		inPosition, 
					 UInt32		requestCount, 
					 void		*buffer, 
					 UInt32		*actualCount)
{    
    HTTPSource* source = inClientData;    
    [source seek:(long)inPosition whence:0];
    //dispatch_sync(dispatch_get_global_queue(0, 0), ^{
        *actualCount = [source read:buffer amount:requestCount];
    //});	
    //NSLog(@"%lu at %lld - %ld", requestCount, inPosition, *actualCount);
    
	//if(requestCount != *actualCount)
    //    return NO;
    
	return noErr;
}

static SInt64
myAudioFile_GetSizeProc(void *inClientData)
{
	HTTPSource* source = inClientData;
    SInt64 len = [source size];
    //NSLog(@"length %lld", len);
	return len;
}

- (void) close
{
	OSStatus			err;
	    
    err = ExtAudioFileDispose(_in);
	if(noErr != err) {
		NSLog(@"Error closing ExtAudioFile");
	}
    
    err = AudioFileClose(_mAudioFile);
    if(noErr != err) {
		NSLog(@"Error closing AudioFileID");
	}

    [_source close];
    [_source release];
}

static const char* CoreAudio_FourCCToString(int32_t error_code)
{
    static char return_string[16];
    uint32_t big_endian_code = htonl(error_code);
    char* big_endian_str = (char*)&big_endian_code;
    // see if it appears to be a 4-char-code
    if(isprint(big_endian_str[0])
       && isprint(big_endian_str[1])
       && isprint(big_endian_str[2])
       && isprint (big_endian_str[3]))
    {
        return_string[0] = '\'';
        return_string[1] = big_endian_str[0];
        return_string[2] = big_endian_str[1];
        return_string[3] = big_endian_str[2];
        return_string[4] = big_endian_str[3];
        return_string[5] = '\'';
        return_string[6] = '\0';
    }
    else if(error_code > -200000 && error_code < 200000)
    {
        // no, format it as an integer
        snprintf(return_string, 16, "%d", error_code);
    }
    else
    {
        // no, format it as an integer but in hex
        snprintf(return_string, 16, "0x%x", error_code);
    }
    return return_string;
}

- (BOOL)open:(HTTPSource*)source;
{
	 //OSStatus						err;

    _source = [source retain];
	OSStatus result = AudioFileOpenWithCallbacks(_source, myAudioFile_ReadProc, NULL, myAudioFile_GetSizeProc, NULL, 0, &_mAudioFile);
        
    if(noErr != result) {
        NSString* strErr = [NSString stringWithCString:CoreAudio_FourCCToString(result) encoding:NSUTF8StringEncoding];
        NSLog(@"%@", strErr);
        return NO;
    }
        
	result = ExtAudioFileWrapAudioFileID(_mAudioFile, false, &_in);    
    //result = ExtAudioFileOpenURL((CFURLRef)[source url], &_in);
	//err = ExtAudioFileOpen(&ref, &_in);
	if(noErr != result) {
		NSLog(@"Error opening file:");
		return NO;
	}
	
	return [self readInfoFromExtAudioFileRef];
}

- (BOOL) readInfoFromExtAudioFileRef
{
	OSStatus						err;
	UInt32							size;
	AudioStreamBasicDescription		asbd;
	
	// Get input file information
	size	= sizeof(asbd);
	ExtAudioFileGetProperty(_in, kExtAudioFileProperty_FileDataFormat, &size, &asbd);
    err = noErr;
	if(err != noErr) {
		ExtAudioFileDispose(_in);
		return NO;
	}
	
	SInt64 total = 0;
	size	= sizeof(total);
	//err		= ExtAudioFileGetProperty(_in, kExtAudioFileProperty_FileLengthFrames, &size, &total);
	if(err != noErr) {
		ExtAudioFileDispose(_in);
		return NO;
	}
	totalFrames = (long)total;
	
	//Is there a way to get bitrate with extAudioFile?
	bitrate				= 0;
	
	// Set our properties
	bitsPerSample		= asbd.mBitsPerChannel;
	channels			= asbd.mChannelsPerFrame;
	frequency			= asbd.mSampleRate;
	
	// mBitsPerChannel will only be set for lpcm formats
	if(0 == bitsPerSample) {
		bitsPerSample = 16;
	}
	
	// Set output format
	AudioStreamBasicDescription		result;
	
	bzero(&result, sizeof(AudioStreamBasicDescription));
	
	result.mFormatID			= kAudioFormatLinearPCM;
	result.mFormatFlags			= kAudioFormatFlagIsSignedInteger | kLinearPCMFormatFlagIsBigEndian;
	
	result.mSampleRate			= frequency;
	result.mChannelsPerFrame	= channels;
	result.mBitsPerChannel		= bitsPerSample;
	
	result.mBytesPerPacket		= channels * (bitsPerSample / 8);
	result.mFramesPerPacket		= 1;
	result.mBytesPerFrame		= channels * (bitsPerSample / 8);
	
	err = ExtAudioFileSetProperty(_in, kExtAudioFileProperty_ClientDataFormat, sizeof(result), &result);
	if(noErr != err) {
		ExtAudioFileDispose(_in);
		return NO;
	}
	
	[self willChangeValueForKey:@"properties"];
	[self didChangeValueForKey:@"properties"];
	
	return YES;
}

- (int) readAudio:(void *)buf frames:(UInt32)frames
{
	OSStatus				err;
	AudioBufferList			bufferList;
	UInt32					frameCount;
	
	// Set up the AudioBufferList
	bufferList.mNumberBuffers				= 1;
	bufferList.mBuffers[0].mNumberChannels	= channels;
	bufferList.mBuffers[0].mData			= buf;
	bufferList.mBuffers[0].mDataByteSize	= frames * channels * (bitsPerSample/8);
	
	// Read a chunk of PCM input (converted from whatever format)
	frameCount	= frames;
	err			= ExtAudioFileRead(_in, &frameCount, &bufferList);
	if(err != noErr) {
		return 0;
	}	
	
	return frameCount;
}

- (long) seek:(long)frame
{
	OSStatus			err;
	
	err = ExtAudioFileSeek(_in, frame);
	if(noErr != err) {
		return -1;
	}
	
	return frame;
}

+ (NSArray *)fileTypes
{
	OSStatus			err;
	UInt32				size;
	NSArray *sAudioExtensions;
	
	size	= sizeof(sAudioExtensions);
	err		= AudioFileGetGlobalInfo(kAudioFileGlobalInfo_AllExtensions, 0, NULL, &size, &sAudioExtensions);
	if(noErr != err) {
		return nil;
	}
	
	return [sAudioExtensions autorelease];
}

+ (NSArray *)mimeTypes
{
	return [NSArray arrayWithObject:@"audio/mpeg"];
}

- (NSDictionary *)properties
{
	return [NSDictionary dictionaryWithObjectsAndKeys:
		[NSNumber numberWithInt:channels],@"channels",
		[NSNumber numberWithInt:bitsPerSample],@"bitsPerSample",
		[NSNumber numberWithInt:bitrate],@"bitrate",
		[NSNumber numberWithFloat:frequency],@"sampleRate",
		[NSNumber numberWithLong:totalFrames],@"totalFrames",
		[NSNumber numberWithBool:YES], @"seekable",
		@"big", @"endian",
		nil];
}


@end
