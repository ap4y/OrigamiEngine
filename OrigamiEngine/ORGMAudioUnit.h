//
//  ORGMAudioUnit.h
//  Origami-engine
//
//  Created by ap4y on 8/5/12.
//
//

#import <Foundation/Foundation.h>

#import <AudioToolbox/AudioToolbox.h>
#import "ORGMQueues.h"

#define CHUNK_SIZE 16 * 1024
#define BUFFER_SIZE 128 * 1024

@interface ORGMAudioUnit : NSObject
- (void)process;
AudioStreamBasicDescription propertiesToASBD(NSDictionary *properties);
@end
