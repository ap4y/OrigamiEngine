#import "M3uContainer.h"

@implementation M3uContainer

#pragma mark - ORMGContainer
+ (NSArray*)fileTypes {
	return [NSArray arrayWithObject:@"m3u"];
}

+ (NSArray*)urlsForContainerURL:(NSURL*)url {
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
    NSCharacterSet* charSet = [NSCharacterSet whitespaceAndNewlineCharacterSet];
    for (NSString* line in [contents componentsSeparatedByString:@"\n"]) {
        NSString *entry = [line stringByTrimmingCharactersInSet:charSet];
        if ([entry hasPrefix:@"#"] || [entry isEqualToString:@""])
			continue;
        [entries addObject:[self urlForPath:entry relativeTo:url]];
    }
	
	return entries;
}

#pragma mark - private

+ (NSURL*)urlForPath:(NSString*)path relativeTo:(NSURL*)baseFileUrl {
	NSRange protocolRange = [path rangeOfString:@"://"];
	if (protocolRange.location != NSNotFound) {
		return [NSURL URLWithString:path];
	}
    
    NSURL* baseUrl = [baseFileUrl URLByDeletingLastPathComponent];
	return [baseUrl URLByAppendingPathComponent:path];
}

@end
