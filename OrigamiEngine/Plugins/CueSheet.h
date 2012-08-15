#import "CueSheetTrack.h"

#define singleFileCueSeparator @"#"

@interface CueSheet : NSObject
@property (copy, nonatomic, readonly) NSArray *tracks;

- (id)initWithURL:(NSURL*)url;
@end
