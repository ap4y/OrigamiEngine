#import "CueSheetMetadataReader.h"
#import "CueSheet.h"

@implementation CueSheetMetadataReader

#pragma mark - ORGMMetadataReader
+ (NSArray*)fileTypes {
	return [NSArray arrayWithObject:@"cue"];
}

+ (NSDictionary*)metadataForURL:(NSURL*)url {
	CueSheet *cuesheet = [[CueSheet alloc] initWithURL:url];
    
    NSDictionary* resultDict = nil;
    for (CueSheetTrack *track in cuesheet.tracks) {
        if ([[url fragment] isEqualToString:[track track]]) {
			resultDict = [NSDictionary dictionaryWithObjectsAndKeys:
                          track.artist, @"artist",
                          track.album, @"album",
                          track.title, @"title",
                          [NSNumber numberWithInt:[track.track intValue]], @"track",
                          track.genre, @"genre",
                          track.year, @"year",
                          nil];
            break;
            
		}
    }
    
    [cuesheet release];
	return resultDict;
}

@end
