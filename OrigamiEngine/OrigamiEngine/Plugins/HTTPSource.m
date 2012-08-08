#import "HTTPSource.h"
@interface HTTPSource () {
	long _byteCount;
    long _byteReaded;
	NSString* _filePath;
    NSFileHandle* _fileHandle;
	NSString *_mimeType;
    NSMutableURLRequest* request;
    NSURLConnection* _urlConnection;
    long long bytesExpected;
}

@end

@implementation HTTPSource
//TODO: refactor this class
static long bytesOffset;

#pragma mark - ORGMEngineObject
+ (NSArray *)fileTypes {
    return nil;
}

+ (NSString *)mimeTypes {
    return nil;    
}

#pragma mark - ORGMSource
+ (NSArray*)schemes {
	return [NSArray arrayWithObject:@"http"];
}

- (NSURL*)url {
	return [request URL];
}

- (long)size {
    return (long)bytesExpected;
}

- (BOOL)open:(NSURL *)url {
    request = [[NSMutableURLRequest requestWithURL:url] retain];
    
    if ([NSThread isMainThread]) {
        _urlConnection = [[NSURLConnection alloc] initWithRequest:request
                                                         delegate:self
                                                 startImmediately:YES];
    }
    else {
        [self performSelector:@selector(mainThreadRequest)
                     onThread:[NSThread mainThread]
                   withObject:nil
                waitUntilDone:NO];
    }
    
    bytesExpected = 0;
    _byteReaded = 0;
    _byteCount = 0;
    _mimeType = @"";
    
    [self prepareCache:[NSString stringWithFormat:@"%x.%@",
                        [[url absoluteString] hash],
                        url.pathExtension]];
    
    while(bytesExpected == 0) {
        [[NSRunLoop mainRunLoop] runMode:NSDefaultRunLoopMode
                              beforeDate:[NSDate distantFuture]];
    }
    
	return YES;
}

- (BOOL)seekable {
	return YES;
}

- (BOOL)seek:(long)position whence:(int)whence {
    _byteReaded = position;
	return YES;
}

- (long)tell {
    return _byteReaded;
}

- (int)read:(void*)buffer amount:(int)amount {
    if (!_fileHandle) {
        return 0;
    }
    
    if (_byteReaded + amount > bytesExpected) {
        return 0;
    }
    
    while(_byteCount < _byteReaded + amount) {
        [[NSRunLoop mainRunLoop] runMode:NSDefaultRunLoopMode
                              beforeDate:[NSDate distantFuture]];
    }
    
    NSData* data = [NSData data];
    int result = 0;
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    @synchronized(_fileHandle) {
        [_fileHandle seekToFileOffset:_byteReaded];
        data = [_fileHandle readDataOfLength:amount];
    }
    
    [data getBytes:buffer length:data.length];
    _byteReaded += data.length;
    
    result = data.length;
    [pool release];
    return result;
}

- (void)close {
    [_urlConnection cancel];
}

#pragma mark - private

- (void)prepareCache:(NSString*)fileName {
    NSArray *paths =
        NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString* dataPath =
        [[paths objectAtIndex:0] stringByAppendingPathComponent:@"StreamCache"];
    
	if (![[NSFileManager defaultManager] fileExistsAtPath:dataPath]) {
        NSError* error = nil;
        if (![[NSFileManager defaultManager] createDirectoryAtPath:dataPath
                                       withIntermediateDirectories:NO
                                                        attributes:nil
                                                             error:&error]) {
            return;
        }
	}
    
    _filePath = [[dataPath stringByAppendingPathComponent:fileName] retain];  
    
    if (![[NSFileManager defaultManager] fileExistsAtPath:_filePath]) {
        if (![[NSFileManager defaultManager] createFileAtPath:_filePath
                                                     contents:nil
                                                   attributes:nil]) {    
            return;
        }
    }
    
    _fileHandle = [[NSFileHandle fileHandleForUpdatingAtPath:_filePath] retain];    
}

- (void)mainThreadRequest {
    _urlConnection = [[NSURLConnection alloc] initWithRequest:request
                                                     delegate:self
                                             startImmediately:YES];
}

- (NSString *)mimeType {
	return _mimeType;
}

- (void)dealloc {
    [_fileHandle closeFile];
    _fileHandle = nil;
    [_filePath release];
    [_fileHandle release];
    [_urlConnection release];
    _urlConnection = nil;
    [request release];
    
	[super dealloc];
}

#pragma mark - NSURLConnection delegate

- (void)connection:(NSURLConnection *)connection
didReceiveResponse:(NSURLResponse *)response {
    _mimeType = response.MIMEType;
    bytesExpected = response.expectedContentLength; 
    
    if ([_fileHandle seekToEndOfFile] == bytesExpected) {
        [_urlConnection cancel];
        _byteCount = (long)bytesExpected;
    }
}

- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data {    
    if (data && _fileHandle) {
        @synchronized(_fileHandle) {
            [_fileHandle seekToEndOfFile];
            
            if (_fileHandle.offsetInFile <= _byteCount + bytesOffset) {                    
                [_fileHandle seekToFileOffset: _byteCount + bytesOffset];                    
                [_fileHandle writeData:data];
            }
            
            _byteCount += data.length;                
        }
    }
}

@end
