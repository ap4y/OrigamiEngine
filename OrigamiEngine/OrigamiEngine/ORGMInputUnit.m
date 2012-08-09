//
//  ORGMInputUnit.m
//  Origami-engine
//
//  Created by ap4y on 8/5/12.
//
//

#import "ORGMInputUnit.h"

#import "HTTPSource.h"
#import "FlacDecoder.h"

@interface ORGMInputUnit () {
    int bytesPerFrame;
    void *inputBuffer;

    BOOL _shouldSeek;
    long seekFrame;
}

@property (nonatomic, strong) NSMutableData* data;
@property (nonatomic, strong) HTTPSource* source;
@property (nonatomic, strong) FlacDecoder* decoder;
@end

@implementation ORGMInputUnit

- (id)init {
    self = [super init];
    if (self) {        
        self.data = [[NSMutableData alloc] init];
        inputBuffer = malloc(CHUNK_SIZE);
    }
    return self;
}

- (void)dealloc {
    free(inputBuffer);
    [super dealloc];
}

#pragma mark - public

- (BOOL)openWithUrl:(NSURL*)url {
    self.source = [[HTTPSource alloc] init];
    if (![_source open:url]) return NO;
    self.decoder = [[FlacDecoder alloc] init];
	if (!_decoder) return NO;    
    if (![_decoder open:_source]) return NO;
    
    int bitsPerSample = [[_decoder.properties objectForKey:@"bitsPerSample"] intValue];
	int channels = [[_decoder.properties objectForKey:@"channels"] intValue];
    bytesPerFrame = (bitsPerSample/8) * channels;
    
    return YES;
}

- (void)close {
    [_decoder close];
}

- (void)process {
    _isProcessing = YES;
    int amountInBuffer = 0; //TODO: check this var
    int framesRead = 0;
    
    do {
        int framesToRead = (CHUNK_SIZE - amountInBuffer)/bytesPerFrame;
        if (_shouldSeek) {
            [_decoder seek:seekFrame];
            _shouldSeek = NO;
        }
        framesRead = [_decoder readAudio:inputBuffer frames:framesToRead];
        amountInBuffer += (framesRead * bytesPerFrame);
        
        dispatch_sync([ORGMQueues lock_queue], ^{
            [_data appendBytes:inputBuffer length:amountInBuffer];
        });
        amountInBuffer = 0;
    } while (framesRead > 0 && _data.length < BUFFER_SIZE);
    
    _isProcessing = NO;
}

- (void)requestNext {
    _isProcessing = YES;
    dispatch_source_merge_data([ORGMQueues buffering_source], 1);
}

- (double)framesCount {
    NSNumber* frames = [_decoder.properties objectForKey:@"totalFrames"];
    return [frames doubleValue];
}

- (void)seek:(double)time {
    seekFrame = time * [[_decoder.properties objectForKey:@"sampleRate"] floatValue];
    _shouldSeek = YES;
}

- (AudioStreamBasicDescription)format {
    return propertiesToASBD(_decoder.properties);
}

#pragma mark - private

@end
