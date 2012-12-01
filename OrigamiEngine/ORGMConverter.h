//
//  ORGMConverter.h
//  Origami-engine
//
//  Created by ap4y on 8/5/12.
//
//

#import "ORGMAudioUnit.h"

/**
 `ORGMConverter` is a subclass of ORGMAudioUnit for converting decoded `PCM` data to `PCM` format compatable with output unit. This class is a wrapper for `AudioConverterRef`, it determines `PCM` format from specified output and input units.
 */
@class ORGMOutputUnit, ORGMInputUnit;
@interface ORGMConverter : ORGMAudioUnit

/**
 Current input unit.
 */
@property (retain, nonatomic, readonly) ORGMInputUnit* inputUnit;

/**
 Current output unit.
 */
@property (retain, nonatomic, readonly) ORGMOutputUnit* outputUnit;

/**
 Buffer with processed `PCM` data compatable with current output unit.
 */
@property (retain, nonatomic, readonly) NSMutableData* convertedData;

/**
 Returns initialized `ORGMConverter` object and specifies input `PCM` format from input unit.
 
 @param inputUnit An input unit to specify input data format.
 
 @return An initialized `ORGMConverter` object.
 */
- (id)initWithInputUnit:(ORGMInputUnit*)inputUnit;

/**
 Specifies output `PCM` format from output unit.
 
 @param outputUnit An output unit to specify output data format.
 
 @return `YES` if success, otherwise `NO`.
 */
- (BOOL)setupWithOutputUnit:(ORGMOutputUnit*)outputUnit;

/**
 Reinitialize instance for a new input `PCM` format.
 
 @discussion This method allow you to reuse current converter instance with different `PCM` formats.
 
 @param inputUnit An input unit to specify input data format.
 @param flush A flag that allows you erase converted data before changing format.
 */
- (void)reinitWithNewInput:(ORGMInputUnit*)inputUnit withDataFlush:(BOOL)flush;
@end
