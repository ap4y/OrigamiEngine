//
//  ORGMCommonProtocols.h
//  OrigamiEngine
//
//  Created by ap4y on 8/8/12.
//
//

#define kErrorDomain @"com.origami.engine.error"
typedef enum : NSInteger {
    ORGMEngineErrorCodesSourceFailed,
    ORGMEngineErrorCodesConverterFailed,
    ORGMEngineErrorCodesDecoderFailed,
    ORGMEngineErrorCodesContainerFailed
} ORGMEngineErrorCodes;

@protocol ORGMEngineObject <NSObject>
@end

/**
 All classes that act as input source must adopt the `ORGMSource` protocol. This protocol contains methods for communication with plugin manager.
 */
@protocol ORGMSource <ORGMEngineObject>

/**
 Returns supported url scheme.
 
 @return A string with supported url scheme.
 */
+ (NSString*)scheme;

/**
 Returns current source file url.
 
 @return Current file url.
 */
- (NSURL*)url;

/**
 Returns source file size.
 
 @return A file size in `bytes`.
 */
- (long)size;

/**
 Opens source file for `read`.
 
 @param url A source file url.
 
 @return `YES` if success, otherwise `NO`.
 */
- (BOOL)open:(NSURL*)url;

/**
 Determines if source is seekable.
 
 @return `YES` if seekable, otherwise `NO`.
 */
- (BOOL)seekable;

/**
 Seeks within a source file in specified direction.
 
 @param position Offset within a source file in `bytes`.
 @param whence Seek base. Supported values `SEEK_SET`, `SEEK_CUR`, or `SEEK_END`.
 
 @return `YES` if success, otherwise `NO`.
 */
- (BOOL)seek:(long)position whence:(int)whence;

/**
 Returns current position offset within a source file.
 
 @return A position offset in `bytes`.
 */
- (long)tell;

/**
 Reads specified amount of data from a source file into provided buffer.
 
 @param buffer A pointer to a buffer. You should allocate enough memory for a buffer.
 @param amount Amount of `bytes` to read from a source.
 
 @return Actual amount of `bytes` read from a source.
 */
- (int)read:(void*)buffer amount:(int)amount;

/**
 Closes a source file. 
 */
- (void)close;
@end

@protocol ORGMContainer <ORGMEngineObject>
+ (NSArray *)fileTypes;
+ (NSArray*)urlsForContainerURL:(NSURL*)url;
@end

@protocol ORGMDecoder <ORGMEngineObject>
@required
+ (NSArray*)fileTypes;
- (NSDictionary*)properties;
- (NSDictionary*)metadata;

- (int)readAudio:(void*)buffer frames:(UInt32)frames;
- (BOOL)open:(id<ORGMSource>)source;
- (long)seek:(long)frame;
- (void)close;
@end