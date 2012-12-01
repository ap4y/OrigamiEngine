//
//  ORGMInputUnit.h
//  Origami-engine
//
//  Created by ap4y on 8/5/12.
//
//

#import "ORGMAudioUnit.h"

/**
 `ORGMInputUnit` is a subclass of ORGMAudioUnit for reading and decoding data from the source.
 */
@interface ORGMInputUnit : ORGMAudioUnit

/**
 Buffer with decoded `PCM` data.
 */
@property (retain, nonatomic, readonly) NSMutableData* data;

/**
 A flag that determines if instance is currently decoding data.
 */
@property (assign, nonatomic, readonly) BOOL isProcessing;

/**
 A flag determines that all input data decoded.
 */
@property (assign, nonatomic, readonly) BOOL endOfInput;

/**
 Open input source and initializes necessary resources. 
 
 @param url A url object to be used as a source path during playback.
 
 @return `YES` if success, otherwise `NO`.
 */
- (BOOL)openWithUrl:(NSURL*)url;

/**
 Closes input unit and corresponding decoder, deallocates unnecessary resources.
 */
- (void)close;

/**
 Triggers event that invokes decoding iteration.
 */
- (void)requestNext;

/**
 Returns current `PCM` audio format.
 
 @return An `ASBD` struct with current audio format.
 */
- (AudioStreamBasicDescription)format;

/**
 Returns current track metadata.
 
 @discussion Dictionary data format depends on the track format. Coverart is included as `NSData` object.
 
 @return Metadata dictionary or `nil` if track don't have metadata.
 */
- (NSDictionary*)metadata;

/**
 Returns frames number for the current source.
 
 @return A frames number for the current source.
 */
- (double)framesCount;

/**
 Seeks to the time within playing track.
 
 @param time Time interval offset in `seconds`.
 */
- (void)seek:(double)time;
@end
