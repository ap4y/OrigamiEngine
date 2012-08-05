#import "HTTPSource.h"

@implementation HTTPSource

static long bytesOffset;

+ (dispatch_queue_t)downloadQueue {
    static dispatch_queue_t _downloadQueue = nil;
    static dispatch_once_t oncePredicate;
    dispatch_once(&oncePredicate, ^{
        _downloadQueue = dispatch_queue_create("downloadQueue", nil);
    });
    
    return _downloadQueue;
}

- (void)notifyData {
    float percent = (float) _byteCount/bytesExpected;
    NSMutableDictionary* info = [NSMutableDictionary dictionary];
    [info setObject:[NSNumber numberWithFloat:percent] forKey:@"percent"];    
    [info setObject:_fileId forKey:@"fileId"];
    
    NSNotification* notification = [NSNotification notificationWithName:@"HTTPSourceDidReceiveData" object:nil userInfo:info];
    [[NSNotificationCenter defaultCenter] postNotification:notification];
}

- (void)prepareCache:(NSString*)fileId {
    NSString* fileName = [NSString stringWithFormat:@"%@.%@", fileId, @"mp3"];
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString* dataPath;
    dataPath = [[paths objectAtIndex:0] stringByAppendingPathComponent:@"StreamCache"];
    
	if (![[NSFileManager defaultManager] fileExistsAtPath:dataPath]) {
        NSError* error = nil;
        if (![[NSFileManager defaultManager] createDirectoryAtPath:dataPath
                                       withIntermediateDirectories:NO
                                                        attributes:nil
                                                             error:&error]) {
            NSLog(@"Unable to create cache directory: %@", error.localizedDescription);    
            return;
        }
	}
    
    _filePath = [[dataPath stringByAppendingPathComponent:fileName] retain];  
    
    if (![[NSFileManager defaultManager] fileExistsAtPath:_filePath]) {
        if (![[NSFileManager defaultManager] createFileAtPath:_filePath contents:nil attributes:nil]) {
            NSLog(@"Unable to create cache file");    
            return;
        }
    }
    
    _fileHandle = [[NSFileHandle fileHandleForUpdatingAtPath:_filePath] retain];    
}

- (BOOL)open:(NSURL *)url {
    
    if (!url)
        return NO;
    
    //cachedData = [[NSMutableData alloc] init]; 
    bytesExpected = 0;
    _byteReaded = 0;
    _byteCount = 0;
    _mimeType = @"";            
    
    _fileId = [[url lastPathComponent] copy];
    [self prepareCache:_fileId];
    
    request = [[NSURLRequest requestWithURL:url] retain];
    [self performSelectorOnMainThread:@selector(mainThreadRequest) withObject:nil waitUntilDone:NO];
    
	return YES;
}

- (void)mainThreadRequest {
    _urlConnection = [[NSURLConnection alloc] initWithRequest:request delegate:self startImmediately:YES];
}

- (NSString *)mimeType {
	return _mimeType;
}

- (BOOL)seekable {
	return YES;
}

- (BOOL)seek:(long)position whence:(int)whence {
    _byteReaded = position;
	return YES;
}

- (long)size {
    return (long)bytesExpected;
}

- (long)tell {
    return _byteReaded;
	//return _byteCount;
}

- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response {        
    _mimeType = response.MIMEType;
    bytesExpected = response.expectedContentLength; 
    
    if ([_fileHandle seekToEndOfFile] == bytesExpected) {
        [_urlConnection cancel];
        _byteCount = (long)bytesExpected;

        [self notifyData];
    }
}

- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data {
    
    if (data && _fileHandle) {        
        dispatch_async([HTTPSource downloadQueue], ^{
            
            @synchronized(_fileHandle) {
                if (_shouldStop)
                    return;
                
                @try {
                    [_fileHandle seekToEndOfFile];
                    
                    if (_fileHandle.offsetInFile <= _byteCount + bytesOffset) {                    
                        [_fileHandle seekToFileOffset: _byteCount + bytesOffset];                    
                        [_fileHandle writeData:data];
                        //NSLog(@"current cache size %ld", _byteCount);
                    }
                    
                    _byteCount += data.length;   
                    
                    dispatch_async(dispatch_get_main_queue(), ^{
                        [self notifyData];
                    });                
                }
                @catch (NSException *exception) {
                    NSLog(@"%@", exception);
                }
            }
        });
        //[cachedData appendData:data];
        //NSLog(@"Appending data");
    }
    else
        NSLog(@"Cant append data");   
}

- (void)connectionDidFinishLoading:(NSURLConnection *)connection {   
}

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error {
    NSLog(@"%@", error.localizedDescription);
}

- (int)read:(void *)buffer amount:(int)amount {
    if (!_fileHandle)
        return 0;

    //if (_byteReaded + amount > bytesExpected)
    //    return 0;
        
    //NSLog(@"trying to read. status have %ld want %ld", _byteCount, _byteReaded + amount);
    while(_byteCount < _byteReaded + amount) {
        [[NSRunLoop currentRunLoop] runMode: NSDefaultRunLoopMode beforeDate:[NSDate distantFuture]];
    }
            
    @try {
        NSData* data = [NSData data];
        int result = 0;
        NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
        @synchronized(_fileHandle) {
            [_fileHandle seekToFileOffset:_byteReaded];
            data = [_fileHandle readDataOfLength:amount];
        }
        
        [data getBytes:buffer length:data.length];
        _byteReaded += data.length;
        //NSLog(@"data length: %i", data.length);
        result = data.length;
        [pool release];
        return result;
    }
    @catch (NSException *exception) {
        return 0;
    }
}

- (void)close {
    _shouldStop = YES;
    
    [_urlConnection cancel];   
    
    [_fileHandle closeFile];
    _fileHandle = nil;
    [_filePath release];
    [_fileHandle release];
    [_urlConnection release];
    _urlConnection = nil;
    [request release];	 
}

- (void)dealloc {
    [self close];	    

	[super dealloc];
}

- (NSURL *)url {
	return [request URL];
}

+ (NSArray *)schemes {
	return [NSArray arrayWithObject:@"http"];
}

@end
