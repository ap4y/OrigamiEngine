//
// ORGMInputUnit.m
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

#import "ORGMInputUnit.h"

#import "ORGMPluginManager.h"

@interface ORGMInputUnit () {
    int bytesPerFrame;
    void *inputBuffer;

    BOOL _shouldSeek;
    long seekFrame;
}

@property (retain, nonatomic) NSMutableData *data;
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

- (BOOL)openWithUrl:(NSURL *)url {
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

- (double)framesCount {
    NSNumber *frames = [_decoder.properties objectForKey:@"totalFrames"];
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

- (int)shiftBytes:(NSUInteger)amount buffer:(void *)buffer {
    int bytesToRead = MIN(amount, _data.length);

    dispatch_sync([ORGMQueues lock_queue], ^{
        memcpy(buffer, _data.bytes, bytesToRead);
        [_data replaceBytesInRange:NSMakeRange(0, bytesToRead) withBytes:NULL length:0];
    });
    
    return bytesToRead;
}

#pragma mark - private

@end
