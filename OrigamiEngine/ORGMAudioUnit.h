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

// default reading chunk size
#define CHUNK_SIZE 16 * 1024
// deault buffer size
#define BUFFER_SIZE 128 * 1024

/**
 Abstract class for playback lifecycle classes.
 */
@interface ORGMAudioUnit : NSObject

/**
 Abstract method. Invokes one processing iteration.
 
 @discussion You should implement this method in subclass.
 */
- (void)process;

/**
 Converts audio properties from `NSDictionary` to `ABSD` format.
 
 @param properties A dictionary object. Supported keys:
    - double `sampleRate`
    - int `bitsPerSample`
    - int `channels`
    - NSString `endian`
    - BOOL `unsigned`
 
 @return A new ABSD struct.
 */
AudioStreamBasicDescription propertiesToASBD(NSDictionary *properties);
@end
