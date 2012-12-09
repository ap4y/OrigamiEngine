//
// ORGMAudioUnit.m
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

#import "ORGMAudioUnit.h"

@interface ORGMAudioUnit ()
@property (nonatomic) BOOL isProcessing;
@end

@implementation ORGMAudioUnit
- (void)process {
    NSString *error = [NSString stringWithFormat:
                       NSLocalizedString(@"You must override %@ in a subclass", nil),
                       NSStringFromSelector(_cmd)];
    @throw [NSException exceptionWithName:NSInternalInconsistencyException
                                   reason:error
                                 userInfo:nil];
}

AudioStreamBasicDescription propertiesToASBD(NSDictionary *properties) {
	AudioStreamBasicDescription asbd;
	asbd.mFormatID = kAudioFormatLinearPCM;
	asbd.mFormatFlags = 0;
    
	asbd.mSampleRate = [[properties objectForKey:@"sampleRate"] doubleValue];
    
	asbd.mBitsPerChannel = [[properties objectForKey:@"bitsPerSample"] intValue];
    
	asbd.mChannelsPerFrame = [[properties objectForKey:@"channels"] intValue];;
	asbd.mBytesPerFrame = (asbd.mBitsPerChannel/8)*asbd.mChannelsPerFrame;
	
	asbd.mFramesPerPacket = 1;
	asbd.mBytesPerPacket = asbd.mBytesPerFrame * asbd.mFramesPerPacket;
	asbd.mReserved = 0;
	
	if ([[properties objectForKey:@"endian"] isEqualToString:@"big"]) {
		asbd.mFormatFlags |= kLinearPCMFormatFlagIsBigEndian;
		asbd.mFormatFlags |= kLinearPCMFormatFlagIsAlignedHigh;
	}
	
	if ([[properties objectForKey:@"unsigned"] boolValue] == NO) {
		asbd.mFormatFlags |= kLinearPCMFormatFlagIsSignedInteger;
	}
	
	return asbd;
}

@end
