//
//  ORGMConverter.h
//  Origami-engine
//
//  Created by ap4y on 8/5/12.
//
//

#import "ORGMAudioUnit.h"

@class ORGMOutputUnit, ORGMInputUnit;
@interface ORGMConverter : ORGMAudioUnit
@property (nonatomic, unsafe_unretained, readonly) ORGMInputUnit* inputUnit;
@property (nonatomic, unsafe_unretained, readonly) ORGMOutputUnit* outputUnit;
@property (nonatomic, strong, readonly) NSMutableData* convertedData;

- (id)initWithInputFormat:(ORGMInputUnit*)inputUnit;
- (BOOL)setupWithOutputUnit:(ORGMOutputUnit*)outputUnit;
- (void)reinitWithNewInput:(ORGMInputUnit*)inputUnit;
@end
