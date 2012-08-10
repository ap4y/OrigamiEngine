#import "CueSheetDecoder.h"
#import "CueSheet.h"

#import "ORGMPluginManager.h"

@interface CueSheetDecoder () {
	long framePosition;
    
	long trackStart;
	long trackEnd;		
}
@property (retain, nonatomic) id<ORGMSource> source;
@property (retain, nonatomic) id<ORGMDecoder> decoder;
@end

@implementation CueSheetDecoder

#pragma mark - ORGMDecoder

+ (NSArray*)fileTypes  {
	return [NSArray arrayWithObject:@"cue"];
}

- (NSDictionary*)properties {
	NSMutableDictionary *properties = [[_decoder properties] mutableCopy];
	[properties setObject:[NSNumber numberWithLong:(trackEnd - trackStart)]
                   forKey:@"totalFrames"];
	return [properties autorelease];
}

- (int)readAudio:(void *)buf frames:(UInt32)frames {
	if (framePosition + frames > trackEnd) {
		frames = trackEnd - framePosition;
	}
    
	if (!frames) {
		return 0;
	}
    
	int n = [_decoder readAudio:buf frames:frames];
	framePosition += n;
	return n;
}

- (BOOL)open:(id<ORGMSource>)s {
	NSURL *url = [s url];
	CueSheet* cuesheet = [[[CueSheet alloc] initWithURL:url] autorelease];
	
    ORGMPluginManager* pluginManager = [ORGMPluginManager sharedManager];
	for (int i = 0; i < cuesheet.tracks.count; i++) {
        CueSheetTrack* track = [cuesheet.tracks objectAtIndex:i];
		if ([track.track isEqualToString:[url fragment]]){
			self.source = [pluginManager sourceForURL:track.url];

			if (![_source open:track.url]) {
				return NO;
			}

			self.decoder = [pluginManager decoderForSource:_source];
			if (![_decoder open:_source]) {
				return NO;
			}

			CueSheetTrack *nextTrack = nil;
			if (i + 1 < [cuesheet.tracks count]) {
				nextTrack = [cuesheet.tracks objectAtIndex:i + 1];
			}

			NSDictionary *properties = [_decoder properties];
			float sampleRate = [[properties objectForKey:@"sampleRate"] floatValue];
			trackStart = [track time] * sampleRate;

			if (nextTrack && [nextTrack.url isEqual:track.url]) {
				trackEnd = [nextTrack time] * sampleRate;
			}
			else {
				trackEnd = [[properties objectForKey:@"totalFrames"] doubleValue];
			}
			[self seek: 0];

			return YES;
		}
	}

	return NO;
}

- (long)seek:(long)frame {
	if (frame > trackEnd - trackStart) {
		return -1;
	}
	
	frame += trackStart;
	framePosition = [_decoder seek:frame];
	return framePosition;
}

- (void)close {
	if (_decoder) {
		[_decoder close];
		[_decoder release];
		_decoder = nil;
	}

	if (_source) {
		[_source release];
		_source = nil;
	}
}

@end
