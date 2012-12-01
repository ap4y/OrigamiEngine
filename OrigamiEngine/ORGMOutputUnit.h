//
//  ORGMOutputUnit.h
//  Origami-engine
//
//  Created by ap4y on 8/5/12.
//
//

#import "ORGMAudioUnit.h"

#import "ORGMConverter.h"

/**
 `ORGMOutputUnit` is a subclass of ORGMAudioUnit for playing converted `PCM` data through the output device. This class gets data from the converter buffer.
 */
@interface ORGMOutputUnit : ORGMAudioUnit

/**
 A flag that determines if instance is currently active.
 */
@property (assign, nonatomic, readonly) BOOL isProcessing;

/**
 Returns initialized `ORGMOutputUnit` object and specifies converter source.
 
 @param converter An converter object used as a data source.
 
 @return An initialized `ORGMOutputUnit` object.
 **/
- (id)initWithConverter:(ORGMConverter*)converter;

/**
 Returns supported `PCM` audio format.
 
 @return An `ASBD` struct with supported audio format.
 */
- (AudioStreamBasicDescription)format;

/**
 Pauses playback throught the output device. Idempotent method.
 */
- (void)pause;

/**
 Resumes playback throught the output device. Idempotent method.
 */
- (void)resume;

/**
 Stops playback throught the output device and deallocates unnecessary resources. Idempotent method.
 */
- (void)stop;

/**
 Converts `frames` number to `seconds` according to the supported format.
 
 @param framesCount `Frames` number to convert to `seconds`.
 
 @return A number of `seconds` for specified number of `frames`.
 */
- (double)framesToSeconds:(double)framesCount;

/**
 Returns amount of played time in `seconds`.
 */
- (double)amountPlayed;

/**
 Seeks to the time within playing track.
 
 @param time Time interval offset in `seconds`.
 */
- (void)seek:(double)time;
@end
