//
//  ORGMInputUnit.m
//  Origami-engine
//
//  Created by ap4y on 8/5/12.
//
//

#import "ORGMInputUnit.h"

#import "ORGMPluginManager.h"

@interface ORGMInputUnit () {
    int bytesPerFrame;
    void *inputBuffer;

    BOOL _shouldSeek;
    long seekFrame;
}

@property (retain, nonatomic) NSMutableData* data;
@property (retain, nonatomic) id<ORGMSource> source;
@property (retain, nonatomic) id<ORGMDecoder> decoder;
@property (assign, nonatomic) BOOL endOfInput;
@end

@implementation ORGMInputUnit

- (id)init {
    self = [super init];
    if (self) {        
        self.data = [NSMutableData data];
        inputBuffer = malloc(CHUNK_SIZE);
        _endOfInput = NO;
    }
    return self;
}

- (void)dealloc {
    [self close];
    free(inputBuffer);
    [_decoder release];
    [_source release];
    [_data release];
    [super dealloc];
}

#pragma mark - public

- (BOOL)openWithUrl:(NSURL*)url {
    self.source = [[ORGMPluginManager sharedManager] sourceForURL:url error:nil];
    if (!_source || ![_source open:url]) return NO;
    self.decoder = [[ORGMPluginManager sharedManager] decoderForSource:_source error:nil];
    if (!_decoder || ![_decoder open:_source]) return NO;
    
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
    int amountInBuffer = 0;
    int framesRead = 0;
        
    do {
        if (_data.length >= BUFFER_SIZE) {
            framesRead = 1;
            break;
        }
        
        if (_shouldSeek) {
            [_decoder seek:seekFrame];
            _shouldSeek = NO;
        }
        int framesToRead = CHUNK_SIZE/bytesPerFrame;
        framesRead = [_decoder readAudio:inputBuffer frames:framesToRead];
        amountInBuffer = (framesRead * bytesPerFrame);
        
        dispatch_sync([ORGMQueues lock_queue], ^{
            [_data appendBytes:inputBuffer length:amountInBuffer];
        });
    } while (framesRead > 0);
    
    if (framesRead <= 0) {
        [self setEndOfInput:YES];
    }
    
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

- (NSDictionary *)metadata {
    return [_decoder metadata];
}

#pragma mark - private

@end
