//
// CueSheet.m
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

#import "CueSheet.h"

#import "ORGMCommonProtocols.h"

@interface CueSheet ()
@property (copy, nonatomic) NSArray *tracks;
@end

@implementation CueSheet

#pragma mark - public

- (id)initWithURL:(NSURL *)url {
	self = [super init];
	if (self) {
		[self parseFileWithUrl:url];
	}
	
	return self;
}

- (void)dealloc {
	[_tracks release];
	[super dealloc];
}

#pragma mark - private
- (void)parseFileWithUrl:(NSURL *)url {
	NSStringEncoding encoding;
	NSError *error = nil;
	NSString *contents = [NSString stringWithContentsOfURL:url
                                              usedEncoding:&encoding
                                                     error:&error];
    if (error) {
        error = nil;
        contents = [NSString stringWithContentsOfURL:url
                                            encoding:NSUTF8StringEncoding
                                               error:&error];
    }
    
    if (error) {
        error = nil;
        contents = [NSString stringWithContentsOfURL:url
                                            encoding:NSWindowsCP1251StringEncoding
                                               error:&error];
	}
    
    if (error) {
        error = nil;
        contents = [NSString stringWithContentsOfURL:url
                                            encoding:NSISOLatin1StringEncoding
                                               error:&error];
	}
    
	if (error || !contents || contents.length <= 0) {
		return;
	}
	
	NSMutableArray *entries = [NSMutableArray array];
    
	NSString *track = nil;
	NSString *path = nil;
	NSString *artist = nil;
	NSString *album = nil;
	NSString *title = nil;
	NSString *genre = nil;
	NSString *year = nil;
	
	BOOL trackAdded = NO;
	NSCharacterSet *whitespace = [NSCharacterSet whitespaceAndNewlineCharacterSet];
    
    for (NSString *line in [contents componentsSeparatedByString:@"\n"]) {
        NSScanner *scanner = [NSScanner scannerWithString:line];
        
        NSString *command;
		if (![scanner scanUpToCharactersFromSet:whitespace intoString:&command]) {
			continue;
		}

        if ([command isEqualToString:@"FILE"]) {
			trackAdded = NO;
            if (![scanner scanString:@"\"" intoString:nil])
                continue;
            
			if (![scanner scanUpToString:@"\"" intoString:&path])
                continue;            
		} else if ([command isEqualToString:@"TRACK"]) {
            trackAdded = NO;            
            if (![scanner scanUpToCharactersFromSet:whitespace intoString:&track])
                continue;
            NSString *type;
            if (![scanner scanUpToCharactersFromSet:whitespace intoString:&type] ||
                ![type isEqualToString:@"AUDIO"]) {
                continue;
            }
        } else if ([command isEqualToString:@"INDEX"]) {
            if (trackAdded || !path)
                continue;
            
            NSString *index = nil;
            if (![scanner scanUpToCharactersFromSet:whitespace intoString:&index] ||
                [index intValue] != 1) {
                continue;
            }
            
            [scanner scanCharactersFromSet:whitespace intoString:nil];            
            NSString *time = nil;
            if (![scanner scanUpToCharactersFromSet:whitespace intoString:&time])
                continue;
            
            NSArray *msf = [time componentsSeparatedByString:@":"];
            if ([msf count] != 3)
                continue;
            
            int minute = [[msf objectAtIndex:0] intValue];
            int second = [[msf objectAtIndex:1] intValue];
            float frame = [[msf objectAtIndex:2] floatValue];
            double seconds = (60*minute) + second + (frame/75);
            
            if (track == nil)
                track = @"01";
            
            [entries addObject:[CueSheetTrack trackWithURL:[self urlForPath:path relativeTo:url]
                                                     track:track
                                                      time:seconds
                                                    artist:artist
                                                     album:album
                                                     title:title
                                                     genre:genre
                                                      year:year]];
            trackAdded = YES;
        } else if ([command isEqualToString:@"PERFORMER"]) {
            if (![scanner scanString:@"\"" intoString:nil])
                continue;
            
            if (![scanner scanUpToString:@"\"" intoString:&artist])
                continue;
        } else if ([command isEqualToString:@"TITLE"]) {
            NSString **titleDest;
            if (!path) {
                titleDest = &album;
            } else {
                titleDest = &title;
            }
            
            if (![scanner scanString:@"\"" intoString:nil])
                continue;
            
            if (![scanner scanUpToString:@"\"" intoString:titleDest])
                continue;
        } else if ([command isEqualToString:@"REM"]) {
            NSString *type;
            if (![scanner scanUpToCharactersFromSet:whitespace intoString:&type])
                continue;
            
            if ([type isEqualToString:@"GENRE"]) {
                if ([scanner scanString:@"\"" intoString:nil]) {
                    if (![scanner scanUpToString:@"\"" intoString:&genre])
                        continue;
                } else {
                    if (![scanner scanUpToCharactersFromSet:whitespace intoString:&genre])
                        continue;
                }
            } else if ([type isEqualToString:@"DATE"]) {
                if (![scanner scanUpToCharactersFromSet:whitespace intoString:&year])
                    continue;
            }
        }
    }
    
	self.tracks = entries;
}

- (NSURL *)urlForPath:(NSString *)path relativeTo:(NSURL *)baseFileUrl {
	NSRange protocolRange = [path rangeOfString:@"://"];
	if (protocolRange.location != NSNotFound) {
		return [NSURL URLWithString:path];
	}

    NSURL *baseUrl = [baseFileUrl URLByDeletingLastPathComponent];        
	return [baseUrl URLByAppendingPathComponent:path];
}

@end
