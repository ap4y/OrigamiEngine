#import "CueSheetTrack.h"

@interface CueSheetTrack ()
@property (retain, nonatomic) NSURL *url;
@property (copy, nonatomic) NSString *track;
@property (copy, nonatomic) NSString *artist;
@property (copy, nonatomic) NSString *album;
@property (copy, nonatomic) NSString *title;
@property (copy, nonatomic) NSString *genre;
@property (copy, nonatomic) NSString *year;
@property (assign, nonatomic) double time;
@end

@implementation CueSheetTrack

+ (id)trackWithURL:(NSURL*)url
             track:(NSString *)track
              time:(double)seconds
            artist:(NSString*)artist
             album:(NSString*)album
             title:(NSString*)title
             genre:(NSString*)genre
              year:(NSString*)year {
    
	return [[[CueSheetTrack alloc] initWithURL:url
                                         track:track
                                          time:seconds
                                        artist:artist
                                         album:album
                                         title:title
                                         genre:genre
                                          year:year] autorelease];
}

- (id)initWithURL:(NSURL*)url
            track:(NSString *)track
             time:(double)seconds
           artist:(NSString *)artist
            album:(NSString *)album
            title:(NSString *)title
            genre:(NSString *)genre
             year:(NSString *)year {

	self = [super init];
	if (self) {
		self.track = track;
		self.url = url;
		self.artist = artist;
		self.album = album;
		self.title = title;
		self.genre = genre;
		self.year = year;
		
		self.time = seconds;
	}
	
	return self;
}

- (void)dealloc {
	[_track release];
	[_url release];
	[_artist release];
	[_album release];
	[_title release];
	[_genre release];
	[_year release];
	
	[super dealloc];
}

@end
