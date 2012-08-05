@interface HTTPSource : NSObject
{
	long _byteCount;
    long _byteReaded;
	NSString* _filePath;
    NSFileHandle* _fileHandle;
	NSString *_mimeType;
    NSMutableURLRequest* request;
    NSURLConnection* _urlConnection;
    __block long long bytesExpected;    
    
    NSString* _fileId;
    BOOL _shouldStop;    
}

+ (NSArray *)schemes;

- (NSURL *)url;
- (NSString *)mimeType;
- (long)size;

- (BOOL)open:(NSURL *)url;
- (BOOL)seekable;
- (BOOL)seek:(long)position whence:(int)whence;
- (long)tell;
- (int)read:(void *)buffer amount:(int)amount;
- (void)close;
@end
