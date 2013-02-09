//
//  ORGMAppDelegate.m
//  OrigamiEngine Example
//
//  Created by Arthur Evstifeev on 9/02/13.
//
//

#import "AppDelegate.h"
#import "PlayerViewController.h"

@interface AppDelegate ()
@property (strong, nonatomic) PlayerViewController *playerViewController;
@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {

    self.playerViewController = [[PlayerViewController alloc] init];
    [_mainView addSubview:_playerViewController.view];
}

@end
