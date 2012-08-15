#import "HTTPSource.h"
@interface HTTPSource () {
	long _byteCount;
    long _byteReaded;	
    long long bytesExpected;
    BOOL _isError;
}
@property (retain, nonatomic) NSURLConnection* urlConnection;
@property (retain, nonatomic) NSURLRequest* request;
@property (retain, nonatomic) NSFileHandle* fileHandle;
@end

@implementation HTTPSource

- (void)dealloc {
    [self close];
    [_fileHandle closeFile];
    [_fileHandle release];
    [_urlConnection release];
    [_request release];
    
	[super dealloc];
}

#pragma mark - ORGMSource
+ (NSString *)scheme {
    return @"http";
}

- (NSURL*)url {
	return [_request URL];
}

- (long)size {
    return (long)bytesExpected;
}

- (BOOL)open:(NSURL*)url {
    self.request = [NSURLRequest requestWithURL:url];    
    NSURLConnection* connection = [[NSURLConnection alloc] initWithRequest:_request
                                                                  delegate:self
                                                          startImmediately:NO];
    self.urlConnection = connection;
    [connection release];
    
    dispatch_sync(dispatch_get_main_queue(), ^{ //fix nsurlconnection delegate
        [_urlConnection start];
    });
    
    bytesExpected = 0;
    _byteReaded = 0;
    _byteCount = 0;
    _isError = NO;
    
    [self prepareCache:[NSString stringWithFormat:@"%x.%@",
                        [[url absoluteString] hash],
                        url.pathExtension]];
    
    while(bytesExpected == 0 && !_isError) {
        [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode
                                 beforeDate:[NSDate distantFuture]];
    }
    
	return YES;
}

- (BOOL)seekable {
	return YES;
}

- (BOOL)seek:(long)position whence:(int)whence {
    switch (whence) {
        case SEEK_SET:
            _byteReaded = position;
            break;
        case SEEK_CUR:
            _byteReaded += position;
            break;
        case SEEK_END:
            _byteReaded = bytesExpected - position;
            break;            
    }
	return YES;
}

- (long)tell {
    return _byteReaded;
}

- (int)read:(void*)buffer amount:(int)amount {
    if (_byteReaded + amount > bytesExpected)
        return 0;

    while(_byteCount < _byteReaded + amount) {
        [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode
                                 beforeDate:[NSDate distantFuture]];
    }
    int result = 0;
    @autoreleasepool {
        NSData* data = nil;
        @synchronized(_fileHandle) {
            [_fileHandle seekToFileOffset:_byteReaded];
            data = [_fileHandle readDataOfLength:amount];
        }
        [data getBytes:buffer length:data.length];
        _byteReaded += data.length;
        
        result = data.length;
    }

    return result;
}

- (void)close {
    [_urlConnection cancel];
}

#pragma mark - private
+ (dispatch_queue_t)cachingQueue {
    static dispatch_queue_t _cachingQueue;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _cachingQueue = dispatch_queue_create("com.origami.httpcache",
                                              DISPATCH_QUEUE_SERIAL);
    });
    return _cachingQueue;
}

- (void)prepareCache:(NSString*)fileName {
    NSArray *paths =
        NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
    NSString* dataPath =
        [[paths objectAtIndex:0] stringByAppendingPathComponent:@"StreamCache"];
    NSFileManager* defaultFileManger = [NSFileManager defaultManager];
	if (![defaultFileManger fileExistsAtPath:dataPath]) {
        if (![defaultFileManger createDirectoryAtPath:dataPath
                          withIntermediateDirectories:NO
                                           attributes:nil
                                                error:nil]) {
            @throw [NSException exceptionWithName:NSInternalInconsistencyException
                                           reason:NSLocalizedString(@"Unable create cache directory", nil)
                                         userInfo:nil];
        }
	}
    
    NSString* filePath = [dataPath stringByAppendingPathComponent:fileName];
    if (![defaultFileManger fileExistsAtPath:filePath]) {
        if (![defaultFileManger createFileAtPath:filePath
                                        contents:nil
                                      attributes:nil]) {    
            @throw [NSException exceptionWithName:NSInternalInconsistencyException
                                           reason:NSLocalizedString(@"Unable create cache file", nil)
                                         userInfo:nil];
        }
    }
    
    self.fileHandle = [NSFileHandle fileHandleForUpdatingAtPath:filePath];
}

#pragma mark - NSURLConnection delegate

- (void)connection:(NSURLConnection *)connection
didReceiveResponse:(NSURLResponse *)response {
    bytesExpected = response.expectedContentLength; 
    
    if ([_fileHandle seekToEndOfFile] == bytesExpected) {
        [_urlConnection cancel];
        _byteCount = (long)bytesExpected;
    }
}

- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data {    
    if (data && _fileHandle) {
        dispatch_async([HTTPSource cachingQueue], ^{
            @synchronized(_fileHandle) {
                [_fileHandle seekToFileOffset:_byteCount];
                [_fileHandle writeData:data];
            }
            _byteCount += data.length;
        });
    }
}

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error {
    _isError = YES;
}

@end
