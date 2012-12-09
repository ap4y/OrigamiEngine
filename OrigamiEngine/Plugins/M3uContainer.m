//
// M3uContainer.m
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

#import "M3uContainer.h"

@implementation M3uContainer

#pragma mark - ORMGContainer
+ (NSArray *)fileTypes {
	return [NSArray arrayWithObject:@"m3u"];
}

+ (NSArray *)urlsForContainerURL:(NSURL*)url {
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
	if (error || !contents) {
		return nil;
	}
	
    NSMutableArray *entries = [NSMutableArray array];
    NSCharacterSet *charSet = [NSCharacterSet whitespaceAndNewlineCharacterSet];
    for (NSString *line in [contents componentsSeparatedByString:@"\n"]) {
        NSString *entry = [line stringByTrimmingCharactersInSet:charSet];
        if ([entry hasPrefix:@"#"] || [entry isEqualToString:@""])
			continue;
        [entries addObject:[self urlForPath:entry relativeTo:url]];
    }
	
	return entries;
}

#pragma mark - private
+ (NSURL *)urlForPath:(NSString *)path relativeTo:(NSURL *)baseFileUrl {
	NSRange protocolRange = [path rangeOfString:@"://"];
	if (protocolRange.location != NSNotFound) {
		return [NSURL URLWithString:path];
	}
    
    NSURL *baseUrl = [baseFileUrl URLByDeletingLastPathComponent];
	return [baseUrl URLByAppendingPathComponent:path];
}

@end
