#import "CueSheetContainer.h"
#import "CueSheet.h"

@implementation CueSheetContainer

#pragma mark - ORMGContainer

+ (NSArray*)fileTypes {
	return [NSArray arrayWithObject:@"cue"];
}

+ (NSArray*)urlsForContainerURL:(NSURL*)url {
	NSMutableArray *tracks = [NSMutableArray array];	
	CueSheet *cuesheet = [[CueSheet alloc] initWithURL:url];
    [cuesheet.tracks enumerateObjectsUsingBlock:^(CueSheetTrack* track,
                                                  NSUInteger idx,
                                                  BOOL *stop) {
        NSString* appendString = [NSString stringWithFormat:@"%@%@",
                                  singleFileCueSeparator, [track track]];
        NSURL* formattedUrl = [track.url URLByAppendingPathComponent:appendString];
        [tracks addObject:formattedUrl];
    }];

	[cuesheet release];
	return tracks;
}

@end
