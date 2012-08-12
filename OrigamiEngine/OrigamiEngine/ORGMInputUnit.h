//
//  ORGMInputUnit.h
//  Origami-engine
//
//  Created by ap4y on 8/5/12.
//
//

#import "ORGMAudioUnit.h"

@interface ORGMInputUnit : ORGMAudioUnit
@property (retain, nonatomic, readonly) NSMutableData* data;
@property (nonatomic, readonly) BOOL isProcessing;
@property (nonatomic, readonly) BOOL endOfInput;

- (BOOL)openWithUrl:(NSURL*)url;
- (void)close;
- (void)requestNext;
- (AudioStreamBasicDescription)format;
- (NSDictionary*)metadata;

- (double)framesCount;
- (void)seek:(double)time;
@end
