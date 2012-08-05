//
//  ORGMOutputUnit.m
//  Origami-engine
//
//  Created by ap4y on 8/5/12.
//
//

#import "ORGMOutputUnit.h"

#import "OutputCoreAudio.h"
#import "ORGMInputUnit.h"

@interface ORGMOutputUnit () {
    AudioStreamBasicDescription _format;
}
@property (nonatomic, strong) OutputCoreAudio* output;
@property (nonatomic, unsafe_unretained) ORGMConverter* converter;
@end

@implementation ORGMOutputUnit

- (id)initWithConverter:(ORGMConverter*)converter {
    self = [super init];
    if (self) {
        self.output = [[OutputCoreAudio alloc] initWithController:self];
        [_output setup];
        
        self.converter = converter;
    }
    return self;
}

#pragma mark - public

- (AudioStreamBasicDescription)format {
    return _format;
}

- (void)process {
    _isProcessing = YES;
    dispatch_async(dispatch_get_main_queue(), ^{
        [_output start];
    });
}

#pragma mark - private

- (int)readData:(void *)ptr amount:(int)amount
{
    if (!_converter) {
        @throw [NSException exceptionWithName:NSInvalidArchiveOperationException
                                       reason:NSLocalizedString(@"Converter is undefined", nli)
                                     userInfo:nil];
    }
    int n;
    long amountPlayed = 0;
    n = amount;
    NSMutableData* convertedData = _converter.convertedData;
    if (convertedData.length < n) {
        n = convertedData.length;
    }
    
    dispatch_sync([ORGMQueues lock_queue], ^{
        memcpy(ptr, convertedData.bytes, n);
        [convertedData replaceBytesInRange:NSMakeRange(0, n) withBytes:NULL length:0];
    });
    amountPlayed += n;
    
    if (convertedData.length <= 0.5*BUFFER_SIZE && !_converter.inputUnit.isProcessing) {
        [_converter.inputUnit requestNext];
    }
    
    return n;
}

- (void)setFormat:(AudioStreamBasicDescription *)f {
	_format = *f;
}

@end
