#include <AudioToolbox/ExtendedAudioFile.h>
#include "HTTPSource.h"

@interface CoreAudioDecoder : NSObject
{
    HTTPSource*           _source;
    AudioFileID             _mAudioFile;
	ExtAudioFileRef			_in;

	int bitrate;
	int bitsPerSample;
	int channels;
	float frequency;
	long totalFrames;
}

+ (NSArray *)mimeTypes;
+ (NSArray *)fileTypes;

- (NSDictionary *)properties;

- (int)readAudio:(void *)buffer frames:(UInt32)frames;

- (BOOL)open:(HTTPSource*)source;
- (long)seek:(long)frame;
- (void)close;

@end
