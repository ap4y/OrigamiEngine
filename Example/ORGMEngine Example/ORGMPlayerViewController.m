//
//  ORGMPlayerViewController.m
//  ORGMEngine Example
//
//  Created by ap4y on 8/8/12.
//  Copyright (c) 2012 ap4y. All rights reserved.
//

#import "ORGMPlayerViewController.h"

@interface ORGMPlayerViewController ()
@property (retain, nonatomic) IBOutlet UITextField *tfUrl;
@property (retain, nonatomic) IBOutlet UILabel *lblPlayedTime;
@property (retain, nonatomic) IBOutlet UISlider *seekSlider;
@property (retain, nonatomic) IBOutlet UIButton *btnPlay;
@property (retain, nonatomic) IBOutlet UIButton *btnPause;
@property (retain, nonatomic) IBOutlet UIButton *btnStop;
@property (retain, nonatomic) ORGMEngine* player;
@property (retain, nonatomic) NSTimer* refreshTimer;
@end

@implementation ORGMPlayerViewController
@synthesize tfUrl;
@synthesize lblPlayedTime;
@synthesize seekSlider;
@synthesize btnPlay;
@synthesize btnPause;
@synthesize btnStop;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        self.player = [[ORGMEngine alloc] init];
        _player.delegate = self;    
    }
    return self;
}

- (void)dealloc {
    [tfUrl release];
    [lblPlayedTime release];
    [seekSlider release];
    [btnPlay release];
    [btnPause release];
    [btnStop release];
    [super dealloc];
}

- (void)refreshUI {
    lblPlayedTime.text = [NSString stringWithFormat:@"%.1fs of %.1fs played",
                          _player.amountPlayed, _player.trackTime];
    seekSlider.value = _player.amountPlayed;
}

- (IBAction)play:(id)sender {
    NSURL* url = [NSURL URLWithString:tfUrl.text];
    [_player playUrl:url];
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
    // Do any additional setup after loading the view from its nib.
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
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

#pragma mark - ORGMEngineDelegate
- (void)engine:(ORGMEngine *)engine didChangeState:(ORGMEngineState)state {
    switch (state) {
        case ORGMEngineStateStopped: {
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
            [btnPause setTitle:NSLocalizedString(@"Pause", nil)
                      forState:UIControlStateNormal];
            [btnPlay setEnabled:NO];
            seekSlider.maximumValue = _player.trackTime;
            break;
        }
    }
}

@end
