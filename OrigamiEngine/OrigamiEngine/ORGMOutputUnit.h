//
//  ORGMOutputUnit.h
//  Origami-engine
//
//  Created by ap4y on 8/5/12.
//
//

#import "ORGMAudioUnit.h"

#import "ORGMConverter.h"

@interface ORGMOutputUnit : ORGMAudioUnit
@property (nonatomic, readonly) BOOL isProcessing;

- (id)initWithConverter:(ORGMConverter*)converter;
- (AudioStreamBasicDescription)format;

- (void)pause;
- (void)resume;

- (double)framesToSeconds:(double)framesCount;
- (double)amountPlayed;

- (int)readData:(void *)ptr amount:(int)amount;
- (void)setFormat:(AudioStreamBasicDescription *)f;
@end
