//
// ORGMConverter.h
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

#import "ORGMAudioUnit.h"

/**
 `ORGMConverter` is a subclass of ORGMAudioUnit for converting decoded `PCM` data to `PCM` format compatable with output unit. This class is a wrapper for `AudioConverterRef`, it determines `PCM` format from specified output and input units.
 */
@class ORGMOutputUnit, ORGMInputUnit;
@interface ORGMConverter : ORGMAudioUnit

/**
 Returns initialized `ORGMConverter` object and specifies input `PCM` format from input unit.
 
 @param inputUnit An input unit to specify input data format.
 
 @return An initialized `ORGMConverter` object.
 */
- (id)initWithInputUnit:(ORGMInputUnit *)inputUnit;

/**
 Specifies output `PCM` format from output unit.
 
 @param outputUnit An output unit to specify output data format.
 
 @return `YES` if success, otherwise `NO`.
 */
- (BOOL)setupWithOutputUnit:(ORGMOutputUnit *)outputUnit;

/**
 Reinitialize instance for a new input `PCM` format.
 
 @discussion This method allow you to reuse current converter instance with different `PCM` formats.
 
 @param inputUnit An input unit to specify input data format.
 @param flush A flag that allows you erase converted data before changing format.
 */
- (void)reinitWithNewInput:(ORGMInputUnit *)inputUnit withDataFlush:(BOOL)flush;

/**
 Returns and removes `amount` of bytes from the beginning of the buffer with converted data.
 
 @param amount Amount of bytes to return.
 @param buffer Destination buffer. Memory should be preallocated.
 
 @return Actual amount of shifted bytes.
 */
- (int)shiftBytes:(NSUInteger)amount buffer:(void *)buffer;

/**
 Identifies if converter is ready for buffering (converter buffer has free space and output unit is not processing)
 
 @return `YES` if converter is ready for buffering.
 */
- (BOOL)isReadyForBuffering;

@end
