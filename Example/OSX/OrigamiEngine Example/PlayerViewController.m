//
//  ORGMPlayerViewController.m
//  OrigamiEngine Example
//
//  Created by Arthur Evstifeev on 9/02/13.
//
//

#import "PlayerViewController.h"
#import <OrigamiEngine/ORGMEngine.h>

@interface PlayerViewController () <ORGMEngineDelegate> {
    NSArray* _playlist;
    NSInteger curTrack;
}
@property (weak) IBOutlet NSTextField *tfUrl;
@property (weak) IBOutlet NSTextField *lblPlayedTime;
@property (weak) IBOutlet NSSlider *seekSlider;
@property (weak) IBOutlet NSButton *btnPlay;
@property (weak) IBOutlet NSButton *btnPause;
@property (weak) IBOutlet NSButton *btnStop;

@property (unsafe_unretained) IBOutlet NSTextView *tvMetadata;
@property (weak) IBOutlet NSImageView *ivCover;

@property (strong, nonatomic) ORGMEngine* player;
@property (strong, nonatomic) NSTimer* refreshTimer;
@end

@implementation PlayerViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        self.player = [[ORGMEngine alloc] init];
        _player.delegate = self;
        _playlist = [[NSArray alloc] initWithObjects:
                     @"file://localhost/Users/ap4y/Documents/music/1.flac",
                     @"file://localhost/Users/ap4y/Documents/music/NEW!.cue#01",
                     @"http://dl.dropbox.com/u/12840562/Beat_Six_-_When_The_Chemicals_Ignite__With_Fuel_.mp3",
                     nil];
    }
    
    return self;
}

- (void)awakeFromNib {
    [super awakeFromNib];
    
    curTrack = 0;
    _tfUrl.stringValue = [_playlist objectAtIndex:curTrack];
    
    self.refreshTimer = [NSTimer scheduledTimerWithTimeInterval:1
                                                         target:self
                                                       selector:@selector(refreshUI)
                                                       userInfo:nil
                                                        repeats:YES];
}

- (void)refreshUI {
    if (_player.currentState == ORGMEngineStatePlaying) {
        _lblPlayedTime.stringValue = [NSString stringWithFormat:@"%.1fs of %.1fs played",
                                      _player.amountPlayed, _player.trackTime];
        _seekSlider.doubleValue = _player.amountPlayed;
    }
}

- (void)playTrack:(NSInteger)track {
    _tfUrl.stringValue = [_playlist objectAtIndex:track];
    NSURL* url = [NSURL URLWithString:_tfUrl.stringValue];
    if (_player.currentState != ORGMEngineStatePlaying) {
        [_player playUrl:url];
    } else {
        [_player setNextUrl:url withDataFlush:YES];
    }
}

- (IBAction)prev:(id)sender {
    curTrack--;
    if (curTrack < 0) {
        curTrack = _playlist.count - 1;
    }
    [self playTrack:curTrack];
}

- (IBAction)next:(id)sender {
    curTrack++;
    if (curTrack >= _playlist.count) {
        curTrack = 0;
    }
    [self playTrack:curTrack];
}

- (IBAction)seek:(id)sender {
    [_player seekToTime:_seekSlider.doubleValue];
}

- (IBAction)volume:(id)sender {
    NSSlider *volumeSlider = sender;
    [_player setVolume:volumeSlider.doubleValue];
}

- (IBAction)play:(id)sender {
    [self playTrack:curTrack];
}

- (IBAction)pause:(id)sender {
    if (_player.currentState == ORGMEngineStatePaused) {
        [_player resume];
    } else if (_player.currentState == ORGMEngineStatePlaying) {
        [_player pause];
    }
}

- (IBAction)stop:(id)sender {
    [_player stop];
}

#pragma mark - ORGMEngineDelegate
- (NSURL *)engineExpectsNextUrl:(ORGMEngine *)engine {
    curTrack++;
    if (curTrack >= _playlist.count) {
        curTrack = 0;
    }
    _tfUrl.stringValue = [_playlist objectAtIndex:curTrack];
    NSURL* url = [NSURL URLWithString:_tfUrl.stringValue];
    return url;
}

- (void)engine:(ORGMEngine *)engine didChangeState:(ORGMEngineState)state {
    switch (state) {
        case ORGMEngineStateStopped: {
            _seekSlider.doubleValue = 0.0;
            _lblPlayedTime.stringValue = @"Waiting...";
            [_btnPlay setEnabled:YES];
            [_btnPause setTitle:NSLocalizedString(@"Pause", nil)];
            break;
        }
        case ORGMEngineStatePaused: {
            [_btnPause setTitle:NSLocalizedString(@"Resume", nil)];
            break;
        }
        case ORGMEngineStatePlaying: {
            NSString* metadata = @"";
            NSDictionary* metadataDict = [_player metadata];
            for (NSString* key in metadataDict.allKeys) {
                if (![key isEqualToString:@"picture"]) {
                    metadata = [metadata stringByAppendingFormat:@"%@: %@\n", key,
                                [metadataDict objectForKey:key]];
                }
            }
            _tvMetadata.string = metadata;
            NSData *data = [metadataDict objectForKey:@"picture"];
            _ivCover.image = data ? [[NSImage alloc] initWithData:data] : nil;
            [_btnPause setTitle:NSLocalizedString(@"Pause", nil)];
            [_btnPlay setEnabled:NO];
            _seekSlider.maxValue = _player.trackTime;
            break;
        }
        case ORGMEngineStateError:
            _tvMetadata.string = [_player.currentError localizedDescription];
            break;
    }
}

@end
