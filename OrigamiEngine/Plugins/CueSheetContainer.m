//
// CueSheetContainer.m
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

#import "CueSheetContainer.h"
#import "CueSheet.h"

@implementation CueSheetContainer

#pragma mark - ORMGContainer

+ (NSArray *)fileTypes {
	return [NSArray arrayWithObject:@"cue"];
}

+ (NSArray *)urlsForContainerURL:(NSURL *)url {
	NSMutableArray *tracks = [NSMutableArray array];	
	CueSheet *cuesheet = [[CueSheet alloc] initWithURL:url];
    [cuesheet.tracks enumerateObjectsUsingBlock:^(CueSheetTrack *track, NSUInteger idx, BOOL *stop) {        
        NSURL *saveURL = track.url;
        NSURL *parentDirectory = [saveURL URLByDeletingLastPathComponent];
        NSString *fileName = [saveURL.lastPathComponent stringByDeletingPathExtension];
        
        NSString *newFileName = [NSString stringWithFormat:@"%@.cue%@%@",
                                 fileName, singleFileCueSeparator, [track track]];
        newFileName = [newFileName stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
        NSURL *formattedUrl = [NSURL URLWithString:newFileName relativeToURL:parentDirectory];
        [tracks addObject:formattedUrl];
    }];

	[cuesheet release];
	return tracks;
}

@end
