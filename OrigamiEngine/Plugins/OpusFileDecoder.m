//
//  OpusFileDecoder.m
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

#import "OpusFileDecoder.h"
#import <opusfile/opusfile.h>

@interface OpusFileDecoder () {
    OggOpusFile *decoder;

    long totalFrames;
}

@property (retain, nonatomic) NSMutableDictionary *metadata;
@property (retain, nonatomic) id<ORGMSource> source;

@end

@implementation OpusFileDecoder
@synthesize source;

- (void)dealloc {
    [self close];
    [_metadata release];
    [source release];
    [super dealloc];
}

#pragma mark - ORGMDecoder

+ (NSArray *)fileTypes {
	return [NSArray arrayWithObjects:@"opus", nil];
}

- (NSDictionary *)properties {
	return [NSDictionary dictionaryWithObjectsAndKeys:
            [NSNumber numberWithInt:2], @"channels",
            [NSNumber numberWithInt:16], @"bitsPerSample",
            [NSNumber numberWithFloat:48000], @"sampleRate",
            [NSNumber numberWithDouble:totalFrames], @"totalFrames",
            [NSNumber numberWithBool:[source seekable]], @"seekable",
            @"little", @"endian",
            nil];
}

- (NSMutableDictionary *)metadata {
    return _metadata;
}

- (int)readAudio:(void *)buffer frames:(UInt32)frames {
    int samples = op_read_stereo(decoder, (opus_int16 *)buffer, frames);

    if (samples < 0) return 0;

    return samples;
}

- (BOOL)open:(id<ORGMSource>)s {
	[self setSource:s];
	
    self.metadata = [NSMutableDictionary dictionary];

    OpusFileCallbacks callbacks = {
        ReadCallback,
        SeekCallback,
        TellCallback,
        0
    };

    int rc;
    decoder = op_open_callbacks(source, &callbacks, NULL, 0, &rc);

    if (rc != 0) return NO;
    
    totalFrames = (long)op_pcm_total(decoder, -1);
    [self parseMetadata];
    
    return YES;
}

- (long)seek:(long)sample {
	return op_pcm_seek(decoder, sample);
}

- (void)close {
    op_free(decoder);
	[source close];

	decoder = NULL;
}

#pragma mark - private

- (void)parseMetadata {

    const OpusTags *tags = op_tags(decoder, -1);
    for (int i = 0; i < tags->comments; i++) {

        const char *comment = tags->user_comments[i];
        NSString *commentValue = [NSString stringWithUTF8String:comment];

        NSRange range = [commentValue rangeOfString:@"="];
        NSString *key = [commentValue substringWithRange:NSMakeRange(0, range.location)];
        NSString *value = [commentValue substringWithRange:
            NSMakeRange(range.location + 1, commentValue.length - range.location - 1)];

        if ([key isEqualToString:@"METADATA_BLOCK_PICTURE"])
        {
            OpusPictureTag picture;
            if (!(opus_picture_tag_parse(&picture, comment))) // 0 on success
            {
                NSData *picture_data = [NSData dataWithBytes:picture.data length:picture.data_length];
                [_metadata setObject:picture_data forKey:@"picture"];
                opus_picture_tag_clear(&picture);
            }
        }
        else {
            [_metadata setObject:value forKey:[key lowercaseString]];
        }
    }
}

#pragma mark - callback

static int ReadCallback(void *stream, unsigned char *ptr, int nbytes) {

    id<ORGMSource> source = stream;
    int result = [source read:ptr amount:nbytes];
	return result;
}

static int SeekCallback(void *stream, opus_int64 offset, int whence) {

	id<ORGMSource> source = stream;
    return [source seek:(long)offset whence:whence] ? 0 : -1;
}

static opus_int64 TellCallback(void *stream) {

	id<ORGMSource> source = stream;
    return [source tell];
}

@end
