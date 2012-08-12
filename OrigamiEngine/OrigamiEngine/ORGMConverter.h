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
@property (unsafe_unretained, nonatomic, readonly) ORGMInputUnit* inputUnit;
@property (unsafe_unretained, nonatomic, readonly) ORGMOutputUnit* outputUnit;
@property (retain, nonatomic, readonly) NSMutableData* convertedData;

- (id)initWithInputFormat:(ORGMInputUnit*)inputUnit;
- (BOOL)setupWithOutputUnit:(ORGMOutputUnit*)outputUnit;
- (void)reinitWithNewInput:(ORGMInputUnit*)inputUnit withDataFlush:(BOOL)flush;
@end
