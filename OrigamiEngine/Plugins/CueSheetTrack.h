@interface CueSheetTrack : NSObject
@property (retain, nonatomic, readonly) NSURL *url;
@property (copy, nonatomic, readonly) NSString *track;
@property (copy, nonatomic, readonly) NSString *artist;
@property (copy, nonatomic, readonly) NSString *album;
@property (copy, nonatomic, readonly) NSString *title;
@property (copy, nonatomic, readonly) NSString *genre;
@property (copy, nonatomic, readonly) NSString *year;
@property (assign, nonatomic, readonly) double time;

+ (id)trackWithURL:(NSURL*)url
             track:(NSString *)track
              time:(double)seconds
            artist:(NSString*)artist
             album:(NSString*)album
             title:(NSString*)title
             genre:(NSString*)genre
              year:(NSString*)year;

- (id)initWithURL:(NSURL*)url
            track:(NSString *)track
             time:(double)seconds
           artist:(NSString *)artist
            album:(NSString *)album
            title:(NSString *)title
            genre:(NSString *)genre
             year:(NSString *)year;
@end
