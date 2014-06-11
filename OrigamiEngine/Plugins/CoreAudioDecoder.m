//
// CoreAudioDecoder.m
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

#import <unistd.h>
#import <AudioToolbox/AudioToolbox.h>

#import "CoreAudioDecoder.h"

@interface CoreAudioDecoder () {
    id<ORGMSource>  _source;
    AudioFileID     _audioFile;
    ExtAudioFileRef _in;

    int bitrate;
    int bitsPerSample;
    int channels;
    float frequency;
    long totalFrames;
}
@property (retain, nonatomic) NSMutableDictionary *metadata;
@end

@implementation CoreAudioDecoder

- (void)dealloc {
    [self close];
    [_source release];
    [_metadata release];
    [super dealloc];
}

#pragma mark - ORGMDecoder
+ (NSArray *)fileTypes {
    OSStatus err;
    UInt32 size;
    NSArray *sAudioExtensions;

    size = sizeof(sAudioExtensions);
    err  = AudioFileGetGlobalInfo(kAudioFileGlobalInfo_AllExtensions, 0, NULL, &size, &sAudioExtensions);
    if (noErr != err) {
        return nil;
    }

    return [sAudioExtensions autorelease];
}

- (NSDictionary *)properties {
    return [NSDictionary dictionaryWithObjectsAndKeys:
            [NSNumber numberWithInt:channels], @"channels",
            [NSNumber numberWithInt:bitsPerSample], @"bitsPerSample",
            [NSNumber numberWithInt:bitrate], @"bitrate",
            [NSNumber numberWithFloat:frequency], @"sampleRate",
            [NSNumber numberWithLong:totalFrames], @"totalFrames",
            [NSNumber numberWithBool:YES], @"seekable",
            @"big", @"endian",
            nil];
}

- (NSMutableDictionary *)metadata {
    return _metadata;
}

- (int)readAudio:(void *)buf frames:(UInt32)frames {
    OSStatus err;
    AudioBufferList bufferList;
    UInt32 frameCount;

    bufferList.mNumberBuffers              = 1;
    bufferList.mBuffers[0].mNumberChannels = channels;
    bufferList.mBuffers[0].mData           = buf;
    bufferList.mBuffers[0].mDataByteSize   = frames * channels * (bitsPerSample/8);

    frameCount = frames;
    err        = ExtAudioFileRead(_in, &frameCount, &bufferList);
    if (err != noErr) {
        return 0;
    }

    return frameCount;
}

- (BOOL)open:(id<ORGMSource>)source {
    self.metadata = [NSMutableDictionary dictionary];
    _source = [source retain];
    OSStatus result = AudioFileOpenWithCallbacks(_source, audioFile_ReadProc, NULL,
                                                 audioFile_GetSizeProc, NULL, 0,
                                                 &_audioFile);

    if (noErr != result) {
        return NO;
    }

    result = ExtAudioFileWrapAudioFileID(_audioFile, false, &_in);
    if (noErr != result) {
        return NO;
    }

    return [self readInfoFromExtAudioFileRef];
}

- (long)seek:(long)frame {
    OSStatus err;

    err = ExtAudioFileSeek(_in, frame);
    if (noErr != err) {
        return -1;
    }

    return frame;
}

- (void)close {
    ExtAudioFileDispose(_in);
    AudioFileClose(_audioFile);
    [_source close];
}

#pragma mark - private

- (BOOL)readInfoFromExtAudioFileRef {
    OSStatus err;
    UInt32 size;
    AudioStreamBasicDescription asbd;

    size = sizeof(asbd);
    err  = ExtAudioFileGetProperty(_in,
            kExtAudioFileProperty_FileDataFormat,
            &size,
            &asbd);
    if (err != noErr) {
        ExtAudioFileDispose(_in);
        return NO;
    }

    bitrate       = 0;
    bitsPerSample = asbd.mBitsPerChannel;
    channels      = asbd.mChannelsPerFrame;
    frequency     = asbd.mSampleRate;

    if(0 == bitsPerSample) {
        bitsPerSample = 16;
    }

    AudioStreamBasicDescription	result;
    bzero(&result, sizeof(AudioStreamBasicDescription));

    result.mFormatID    = kAudioFormatLinearPCM;
    result.mFormatFlags = kAudioFormatFlagIsSignedInteger | kLinearPCMFormatFlagIsBigEndian;

    result.mSampleRate       = frequency;
    result.mChannelsPerFrame = channels;
    result.mBitsPerChannel   = bitsPerSample;

    result.mBytesPerPacket  = channels * (bitsPerSample / 8);
    result.mFramesPerPacket = 1;
    result.mBytesPerFrame   = channels * (bitsPerSample / 8);

    err = ExtAudioFileSetProperty(_in, kExtAudioFileProperty_ClientDataFormat,
            sizeof(result), &result);
    if(noErr != err) {
        ExtAudioFileDispose(_in);
        return NO;
    }

    AudioFileID audioFile;
    size = sizeof(AudioFileID);
    err = ExtAudioFileGetProperty(_in,
            kExtAudioFileProperty_AudioFile,
            &size,
            &audioFile);

    if (err == noErr) {
        self.metadata = [self metadataForFile:audioFile];
    }

    Float64 total = 0;
    size = sizeof(total);
    err = AudioFileGetProperty(audioFile, kAudioFilePropertyEstimatedDuration, &size, &total);
    if(err == noErr) totalFrames = total * frequency;

    return YES;
}

- (NSMutableDictionary *)metadataForFile:(AudioFileID)audioFile {

    if ([_source isKindOfClass:NSClassFromString(@"HTTPSource")] &&
        [[[_source url] pathExtension] isEqualToString:@"mp3"]) {

        uint16_t data;
        [_source seek:0 whence:SEEK_SET];
        [_source read:&data amount:2];
        if (data != 17481) return nil; // ID == 17481
    }

    NSMutableDictionary *result = nil;
    UInt32 dataSize = 0;
    OSStatus err;

    err = AudioFileGetPropertyInfo(audioFile,
                                   kAudioFilePropertyInfoDictionary,
                                   &dataSize,
                                   0);

    if (err != noErr) return result;

    CFDictionaryRef dictionary;
    err = AudioFileGetProperty(audioFile, kAudioFilePropertyInfoDictionary, &dataSize, &dictionary);
    if (err != noErr) return result;

    result = [NSMutableDictionary dictionaryWithDictionary:(NSDictionary *)dictionary];
    CFRelease(dictionary);

    err = AudioFileGetPropertyInfo(audioFile,
                                   kAudioFilePropertyAlbumArtwork,
                                   &dataSize,
                                   0);
    NSData *image;
    if (err == noErr) {
        AudioFileGetProperty(audioFile,
                             kAudioFilePropertyAlbumArtwork,
                             &dataSize,
                             &image);
        if (image) {
            [self.metadata setObject:image forKey:@"picture"];
            CFRelease(image);
        }

    } else if ((image = [self imageDataFromID3Tag:audioFile])) {
        [self.metadata setObject:image forKey:@"picture"];
    }

    return result;
}

- (NSData *)imageDataFromID3Tag:(AudioFileID)audioFile {

    OSStatus err;

    UInt32 propertySize = 0;
    AudioFileGetPropertyInfo(audioFile,
            kAudioFilePropertyID3Tag,
            &propertySize,
            0);

    char *rawID3Tag = (char *)malloc(propertySize);
    err = AudioFileGetProperty(audioFile,
            kAudioFilePropertyID3Tag,
            &propertySize,
            rawID3Tag);

    if (err != noErr) {
        free(rawID3Tag);
        return nil;
    }

    UInt32 id3TagSize = 0;
    UInt32 id3TagSizeLength = sizeof(id3TagSize);
    AudioFormatGetProperty(kAudioFormatProperty_ID3TagSize,
            propertySize,
            rawID3Tag,
            &id3TagSizeLength,
            &id3TagSize);

    CFDictionaryRef id3Dict;
    AudioFormatGetProperty(kAudioFormatProperty_ID3TagToDictionary,
            propertySize,
            rawID3Tag,
            &id3TagSize,
            &id3Dict);

    NSDictionary *tagDict = [NSDictionary dictionaryWithDictionary:(NSDictionary *)id3Dict];
    free(rawID3Tag);
    CFRelease(id3Dict);

    NSDictionary *apicDict = tagDict[@"APIC"];
    if (!apicDict) return nil;

    NSString *picKey      = [[apicDict allKeys] lastObject];
    NSDictionary *picDict = apicDict[picKey];
    if (!picDict) return nil;

    return picDict[@"data"];
}

#pragma mark - callback functions

static OSStatus audioFile_ReadProc(void *inClientData,
                                   SInt64 inPosition,
                                   UInt32 requestCount,
                                   void *buffer,
                                   UInt32 *actualCount) {
    id<ORGMSource> source = inClientData;
    [source seek:(long)inPosition whence:0];
    *actualCount = [source read:buffer amount:requestCount];

    return noErr;
}

static SInt64 audioFile_GetSizeProc(void *inClientData) {
    id<ORGMSource> source = inClientData;
    SInt64 len = [source size];
    return len;
}

@end
