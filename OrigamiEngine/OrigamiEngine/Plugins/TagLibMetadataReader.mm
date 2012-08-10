#import "TagLibMetadataReader.h"
#import "fileref.h"
#import "tag.h"
#import "mpegfile.h"
#import "id3v2tag.h"
#import "attachedpictureframe.h"

@implementation TagLibMetadataReader

#pragma mark - ORGMMetadataReader
+ (NSArray*)fileTypes {
	return [NSArray arrayWithObjects:@"ogg", @"mpc", @"flac", @"m4a", @"mp3", nil];
}

+ (NSDictionary*)metadataForURL:(NSURL*)url {
	NSMutableDictionary *dict = [[NSMutableDictionary alloc] init];
	
	TagLib::FileRef f((const char *)[[url path] UTF8String], false);
	if (!f.isNull()) {
		const TagLib::Tag* tag = f.tag();
		
		if (tag) {
			TagLib::String artist, title, album, genre, comment;
			int year, track;
			
			artist = tag->artist();
			title = tag->title();;
			album = tag->album();
			genre = tag->genre();
			comment = tag->comment();
			
			year = tag->year();
			[dict setObject:[NSNumber numberWithInt:year] forKey:@"year"];
			
			track = tag->track();
			[dict setObject:[NSNumber numberWithInt:track] forKey:@"track"];
			
			if (!artist.isNull()) {
				[dict setObject:[NSString stringWithUTF8String:artist.toCString(true)]
                         forKey:@"artist"];
            }

			if (!album.isNull()) {
				[dict setObject:[NSString stringWithUTF8String:album.toCString(true)]
                         forKey:@"album"];
            }
			
			if (!title.isNull()) {
				[dict setObject:[NSString stringWithUTF8String:title.toCString(true)]
                         forKey:@"title"];
            }
			
			if (!genre.isNull()) {
				[dict setObject:[NSString stringWithUTF8String:genre.toCString(true)]
                         forKey:@"genre"];
            }
		}
		
		NSString* imageCacheTag = [NSString stringWithFormat:@"%@-%@-%@-%@",
                                   [dict objectForKey:@"album"],
                                   [dict objectForKey:@"artist"],
                                   [dict objectForKey:@"genre"],
                                   [dict objectForKey:@"year"]];
		NSData* image = [NSData dataWithContentsOfFile:imageCacheTag];
		
		if (!image) {
			TagLib::MPEG::File *mf = dynamic_cast<TagLib::MPEG::File*>(f.file());
			if (mf) {
				TagLib::ID3v2::Tag *tag = mf->ID3v2Tag();
				if (tag) {
					TagLib::ID3v2::FrameList pictures =
                        mf->ID3v2Tag()->frameListMap()["APIC"];
					if (!pictures.isEmpty()) {
						TagLib::ID3v2::AttachedPictureFrame* pic =
                            static_cast<TagLib::ID3v2::AttachedPictureFrame*>(pictures.front());
						
						NSData* data = [[NSData alloc] initWithBytes:pic->picture().data()
                                                              length:pic->picture().size()];
						image = [NSData dataWithData:data];
						[data release];
					}
				}
			}
		}

		if (!image) {
			static NSString* lastImagePath = nil;
			static NSString* lastCacheTag = nil;
						
			NSString* path = [[url path] stringByDeletingLastPathComponent];

            if (lastImagePath) {
                [lastImagePath release];
            }
			lastImagePath = [path retain];
				
            if (lastCacheTag) {
                [lastCacheTag release];
            }				
            lastCacheTag = [imageCacheTag retain];
				
            NSArray *fileNames =
                [[NSFileManager defaultManager] contentsOfDirectoryAtPath:path
                                                                    error:nil];
            NSArray *imageFileNames =
                [fileNames pathsMatchingExtensions:[NSArray arrayWithObjects:
                                                    @"jpg", @"png", nil]];

            for (NSString *fileName in imageFileNames) {
                if ([TagLibMetadataReader isCoverFile:fileName]) {
                    imageCacheTag = [path stringByAppendingPathComponent:fileName];
                    image = [NSData dataWithContentsOfFile:imageCacheTag];
                    break;
                }
            }
		}
		
		if (image) {
			[dict setObject:image forKey:@"albumArt"];
		}
	}

	return [dict autorelease];
}

#pragma mark - private

+ (BOOL)isCoverFile:(NSString*)fileName {
    for (NSString *coverFileName in [TagLibMetadataReader coverNames]) {
        NSString* nameWithoutExtension =
            [[fileName lastPathComponent] stringByDeletingPathExtension];
        if ([[nameWithoutExtension lowercaseString] hasSuffix:coverFileName]) {
			return true;
		}
    }

	return false;
			
}

+ (NSArray *)coverNames {
	return [NSArray arrayWithObjects:@"cover", @"folder", @"album", @"front", nil];
}

@end
