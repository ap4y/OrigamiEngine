//
//  ORGMPlayerViewController.m
//  ORGMEngine Example
//
//  Created by ap4y on 8/8/12.
//  Copyright (c) 2012 ap4y. All rights reserved.
//

#import "ORGMPlayerViewController.h"
#import "ORGMEngine.h"

@interface ORGMPlayerViewController ()
@property (retain, nonatomic) IBOutlet UITextField *tfUrl;
@property (retain, nonatomic) IBOutlet UILabel *lblPlayedTime;
@property (retain, nonatomic) IBOutlet UISlider *seekSlider;
@property (retain, nonatomic) ORGMEngine* player;
@end

@implementation ORGMPlayerViewController
@synthesize tfUrl;
@synthesize lblPlayedTime;
@synthesize seekSlider;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        self.player = [[ORGMEngine alloc] init];
    }
    return self;
}

- (IBAction)play:(id)sender {
    NSURL* url = [NSURL URLWithString:tfUrl.text];
    [_player playUrl:url];
}

- (IBAction)pause:(id)sender {
    [_player pause];
}

- (IBAction)stop:(id)sender {
    [_player stop];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
}

- (void)viewDidUnload
{
    [self setTfUrl:nil];
    [self setLblPlayedTime:nil];
    [self setSeekSlider:nil];
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

- (void)dealloc {
    [tfUrl release];
    [lblPlayedTime release];
    [seekSlider release];
    [super dealloc];
}
@end
