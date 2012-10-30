#import "CueSheetContainer.h"
#import "CueSheet.h"

@implementation CueSheetContainer

#pragma mark - ORMGContainer

+ (NSArray *)fileTypes {
	return [NSArray arrayWithObject:@"cue"];
}

+ (NSArray *)urlsForContainerURL:(NSURL *)url {
	NSMutableArray *tracks = [NSMutableArray array];	
	CueSheet *cuesheet = [[CueSheet alloc] initWithURL:url];
    [cuesheet.tracks enumerateObjectsUsingBlock:^(CueSheetTrack* track,
                                                  NSUInteger idx,
                                                  BOOL *stop) {        
        NSURL* saveURL = track.url;
        NSURL* parentDirectory = [saveURL URLByDeletingLastPathComponent];
        NSString* fileName = [saveURL.lastPathComponent stringByDeletingPathExtension];
        
        NSString *newFileName = [NSString stringWithFormat:@"%@.cue%@%@",
                                 fileName, singleFileCueSeparator, [track track]];
        NSURL* formattedUrl = [NSURL URLWithString:newFileName relativeToURL:parentDirectory];        
        [tracks addObject:formattedUrl];
    }];

	[cuesheet release];
	return tracks;
}

@end
