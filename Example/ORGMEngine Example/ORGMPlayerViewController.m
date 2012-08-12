//
//  ORGMPlayerViewController.m
//  ORGMEngine Example
//
//  Created by ap4y on 8/8/12.
//  Copyright (c) 2012 ap4y. All rights reserved.
//

#import "ORGMPlayerViewController.h"

@interface ORGMPlayerViewController () {
    NSArray* _playlist;
    NSInteger curTrack;
}
@property (strong, nonatomic) IBOutlet UITextField *tfUrl;
@property (strong, nonatomic) IBOutlet UILabel *lblPlayedTime;
@property (strong, nonatomic) IBOutlet UISlider *seekSlider;
@property (strong, nonatomic) IBOutlet UIButton *btnPlay;
@property (strong, nonatomic) IBOutlet UIButton *btnPause;
@property (strong, nonatomic) IBOutlet UIButton *btnStop;
@property (strong, nonatomic) IBOutlet UITextView *tvMetadata;
@property (strong, nonatomic) IBOutlet UIImageView *ivCover;
@property (strong, nonatomic) ORGMEngine* player;
@property (strong, nonatomic) NSTimer* refreshTimer;
@end

@implementation ORGMPlayerViewController
@synthesize tfUrl;
@synthesize lblPlayedTime;
@synthesize seekSlider;
@synthesize btnPlay;
@synthesize btnPause;
@synthesize btnStop;
@synthesize tvMetadata;
@synthesize ivCover;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        self.player = [[ORGMEngine alloc] init];
        _player.delegate = self;
        _playlist = [[NSArray alloc] initWithObjects:
                     @"http://readyshare.routerlogin.net/shares/USB_Storage/Jack%20Johnson%20-%202010%20-%20To%20The%20Sea%20[FLAC]/01%20Jack%20Johnson%20-%20You%20And%20Your%20Heart.flac",
                     @"http://readyshare.routerlogin.net/shares/USB_Storage/LAMA%20-%20NEW!/NEW!.cue#01",
                     @"http://dl.dropbox.com/u/12840562/Beat_Six_-_When_The_Chemicals_Ignite__With_Fuel_.mp3",
                     nil];
    }
    return self;
}


- (void)refreshUI {
    if (_player.currentState == ORGMEngineStatePlaying) {
        lblPlayedTime.text = [NSString stringWithFormat:@"%.1fs of %.1fs played",
                              _player.amountPlayed, _player.trackTime];
        seekSlider.value = _player.amountPlayed;
    }
}

- (void)playTrack:(NSInteger)track {
    tfUrl.text = [_playlist objectAtIndex:track];
    NSURL* url = [NSURL URLWithString:tfUrl.text];
    [_player playUrl:url];
}

- (IBAction)prev:(id)sender {
    [_player stop];
    curTrack--;
    if (curTrack < 0) {
        curTrack = _playlist.count - 1;
    }
    [self playTrack:curTrack];
}

- (IBAction)next:(id)sender {
    [_player stop];
    curTrack++;
    if (curTrack >= _playlist.count) {
        curTrack = 0;
    }
    [self playTrack:curTrack];
}

- (IBAction)seek:(id)sender {
    [_player seekToTime:seekSlider.value];
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

- (void)viewDidLoad
{
    [super viewDidLoad];
    curTrack = 0;
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    self.refreshTimer = [NSTimer scheduledTimerWithTimeInterval:1
                                                         target:self
                                                       selector:@selector(refreshUI)
                                                       userInfo:nil
                                                        repeats:YES];
}

- (void)viewWillDisappear:(BOOL)animated {
    [super viewWillDisappear:animated];
    [_refreshTimer invalidate];
}

- (void)viewDidUnload
{
    [self setTfUrl:nil];
    [self setLblPlayedTime:nil];
    [self setSeekSlider:nil];
    [self setBtnPlay:nil];
    [self setBtnPause:nil];
    [self setBtnStop:nil];
    [self setTvMetadata:nil];
    [self setIvCover:nil];
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

#pragma mark - ORGMEngineDelegate
- (NSURL *)engineIsExpectNextUrl:(ORGMEngine *)engine {
    curTrack--;
    if (curTrack < 0) {
        curTrack = _playlist.count - 1;
    }
    tfUrl.text = [_playlist objectAtIndex:curTrack];
    NSURL* url = [NSURL URLWithString:tfUrl.text];
    return url;
}

- (void)engine:(ORGMEngine *)engine didChangeState:(ORGMEngineState)state {
    switch (state) {
        case ORGMEngineStateStopped: {
            seekSlider.value = 0.0;
            lblPlayedTime.text = @"Waiting...";
            [btnPlay setEnabled:YES];
            [btnPause setTitle:NSLocalizedString(@"Pause", nil)
                      forState:UIControlStateNormal];
            break;
        }
        case ORGMEngineStatePaused: {
            [btnPause setTitle:NSLocalizedString(@"Resume", nil)
                      forState:UIControlStateNormal];
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
            tvMetadata.text = metadata;
            NSData* data = [metadataDict objectForKey:@"picture"];
            ivCover.image = data ? [UIImage imageWithData:data] : nil;
            [btnPause setTitle:NSLocalizedString(@"Pause", nil)
                      forState:UIControlStateNormal];
            [btnPlay setEnabled:NO];
            seekSlider.maximumValue = _player.trackTime;
            break;
        }
    }
}

@end
