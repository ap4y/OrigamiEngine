//
//  ORGMOutputUnit.m
//  Origami-engine
//
//  Created by ap4y on 8/5/12.
//
//

#import "ORGMOutputUnit.h"

#import "ORGMCoreAudioOutput.h"
#import "ORGMInputUnit.h"

@interface ORGMOutputUnit () {
    AudioStreamBasicDescription _format;
    unsigned long long _amountPlayed;
}
@property (retain, nonatomic) ORGMConverter* converter;
@end

@implementation ORGMOutputUnit

- (id)initWithConverter:(ORGMConverter*)converter {
    self = [super init];
    if (self) {
        ORGMCoreAudioOutput* output =
            [[ORGMCoreAudioOutput alloc] initWithController:self];
        self.output = output;
        [output release];
        
        [_output setup];
        
        self.converter = converter;
        _amountPlayed = 0;
    }
    return self;
}

- (void)dealloc {
    [self stop];
    [_output release];
    [super dealloc];
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

- (void)pause {
    [_output pause];
}

- (void)resume {
    [_output resume];
}

- (void)stop {
    [_output stop];
}

- (double)framesToSeconds:(double)framesCount {
	return (framesCount/_format.mSampleRate);
}

- (double)amountPlayed {
	return (_amountPlayed/_format.mBytesPerFrame)/(_format.mSampleRate);
}

- (void)seek:(double)time {
	_amountPlayed = time*_format.mBytesPerFrame*(_format.mSampleRate);
}

#pragma mark - callbacks

- (int)readData:(void *)ptr amount:(int)amount {
    if (!_converter) {
        @throw [NSException exceptionWithName:NSInvalidArchiveOperationException
                                       reason:NSLocalizedString(@"Converter is undefined", nli)
                                     userInfo:nil];
    }
    int n;
    n = amount;
    NSMutableData* convertedData = _converter.convertedData;
    if (convertedData.length < n) {
        n = convertedData.length;
    }
    
    dispatch_sync([ORGMQueues lock_queue], ^{
        memcpy(ptr, convertedData.bytes, n);
        [convertedData replaceBytesInRange:NSMakeRange(0, n) withBytes:NULL length:0];
    });
    _amountPlayed += n;
    
    if (convertedData.length <= 0.5*BUFFER_SIZE && !_converter.inputUnit.isProcessing) {
        [_converter.inputUnit requestNext];
    }
    
    return n;
}

- (void)setFormat:(AudioStreamBasicDescription *)f {
	_format = *f;
}

@end
