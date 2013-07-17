//
// ORGMAudioUnit.h
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

#import <AudioToolbox/AudioToolbox.h>
#import "ORGMQueues.h"

// default reading chunk size
#define CHUNK_SIZE 16 * 1024
// deault buffer size
#define BUFFER_SIZE 128 * 1024

/**
 Specifies format of the PCM output
 */
typedef enum : NSUInteger {
    ORGMOutputFormatDefault,
    ORGMOutputFormat24bit
} ORGMEngineOutputFormat;

/**
 Abstract class for playback lifecycle classes.
 */
@interface ORGMAudioUnit : NSObject

/**
 Invokes one processing iteration.
 
 @discussion You should implement this method in subclass of `ORGMAudioUnit`.
 */
- (void)process;

/**
 Converts audio properties from `NSDictionary` to `ASBD` format.
 
 @param properties A dictionary object. Supported keys:
    - double `sampleRate`
    - int `bitsPerSample`
    - int `channels`
    - NSString `endian`
    - BOOL `unsigned`
 
 @return A new ASBD struct.
 */
AudioStreamBasicDescription propertiesToASBD(NSDictionary *properties);
@end
