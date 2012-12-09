//
// ORGMEngine.h
//
// Copyright (c) 2012 ap4y (lod@pisem.net)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#import <Foundation/Foundation.h>
 
@protocol ORGMEngineDelegate;

/**
 Specifies states of the engine.
 */
typedef enum : NSInteger {
    ORGMEngineStateStopped,
    ORGMEngineStatePlaying,
    ORGMEngineStatePaused,
    ORGMEngineStateError
} ORGMEngineState;

/**
 `ORGMEngine` is a facade for audio playing functionality (decoding, converting, output). If you need common audio player functionality, you should use this class. In specific usecases (such as only decoding, metadata reading etc.) it would be more efficient to use dedicated functionality from other classes.
 */
@interface ORGMEngine : NSObject

/**
 Current state of the engine instance.
 */
@property (assign, nonatomic, readonly) ORGMEngineState currentState;

/**
 Current error of the instance. 
 
 @discussion Value will be provided only for the `ORGMEngineStateError`, with other states this propertie will return `nil`.
 */
@property (retain, nonatomic, readonly) NSError *currentError;

/**
 The object that conforms ORGMEngineDelegate protocol and acts as the delegate.
 */
@property (unsafe_unretained, nonatomic) id<ORGMEngineDelegate> delegate;

/**
 Starts new playback process from corresponding source.
 
 @param url The url object to be used as a source path during playback.
 */
- (void)playUrl:(NSURL *)url;

/**
 Pauses the playback. 
 
 @discussion This method will pause only output processing, decoding and converting will be still active. Only have effect during the `ORGMEngineStatePlaying` state.
 */
- (void)pause;

/**
 Resumes the playback.

 @discussion Only have effect during the `ORGMEngineStatePaused` state.
 */
- (void)resume;

/**
 Stops the playback. 
 
 @discussion This will halt all playback lifecycle and will destroy underlying objects.
 */
- (void)stop;

/**
 Provides current track length.
 
 @return Overall track time in `seconds`.
 */
- (double)trackTime;

/**
 Provides played time.
 
 @return Played amount in `seconds`.
 */
- (double)amountPlayed;

/**
 Returns current track metadata. 
 
 @discussion Dictionary data format depends on the track format. Coverart is included as `NSData` object.
 
 @return Metadata dictionary or `nil` if track don't have metadata.
 */
- (NSDictionary *)metadata;

/**
 Provides ability to seek within playing track.
 
  @param time Time interval offset in `seconds`.
 */
- (void)seekToTime:(double)time;

/**
 Provides next/previous functionaly. 

 @discussion This method allows to implement prev/next functionality without significant memory overhead, because it will only create new input source and will reuse allocated converter and output unit. The `flush` flag determines undelying switching process. If `flush` is `YES`, than accumulated output buffer will be erased before switching to the next track. This will result in a small silence interval between tracks, because engine have to decode initial data, overall switch can be faster. If `flush` is `NO`, than engine will switch tracks only after playing data from output buffer. This will allow to decode initial data for next track.
 
 @param url The url object to be used as a source path during playback.
 @param flush A flag that allows you erase accumulated data before changing the track.
 */
- (void)setNextUrl:(NSURL *)url withDataFlush:(BOOL)flush;
@end

/**
 The delegate of a ORGMEngine object must adopt the `ORGMEngineDelegate` protocol. This protocol allows you to get state change notifications and implement continious playback. */
@protocol ORGMEngineDelegate <NSObject>

/**
 Asks the delegate for the next track url.
 
 @discussion This method provides continious playback functionality. When decoder encounters end of the input source, engine will try to request next track via this method. If new correct url is provided, than engine will pre-buffer new data, so track switching will be smooth. If `nil` or incorrect url is provided, than engine will switch to the stop state at the end of the current track.
 
 @param engine The engine object requesting this information.
 
 @return The url object to be used as a source path during playback.
 */
- (NSURL *)engineExpectsNextUrl:(ORGMEngine *)engine;

@optional

/**
 Notifies the delegate about current state changes.
 
 @param engine The engine object posting this information.
 @param state New state of the engine object.
 */
- (void)engine:(ORGMEngine *)engine didChangeState:(ORGMEngineState)state;
@end
