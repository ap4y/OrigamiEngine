//
// FileSource.m
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

#import "FileSource.h"

@interface FileSource () {
    FILE *_fd;
}
@property (strong, nonatomic) NSURL *url;
@end

@implementation FileSource

- (void)dealloc {
	[self close];
}

#pragma mark - ORGMSource
+ (NSString *)scheme {
    return @"file";
}

- (NSURL *)url {
	return _url;
}

- (long)size {
    long curpos = ftell(_fd);
    fseek (_fd, 0, SEEK_END);
    long size = ftell(_fd);
    fseek(_fd, curpos, SEEK_SET);
	return size;
}

- (BOOL)open:(NSURL *)url {
	[self setUrl:url];
	_fd = fopen([[url path] UTF8String], "r");
	return (_fd != NULL);
}

- (BOOL)seekable {
	return YES;
}

- (BOOL)seek:(long)position whence:(int)whence {
	return (fseek(_fd, position, whence) == 0);
}

- (long)tell {
    return ftell(_fd);
}

- (int)read:(void *)buffer amount:(int)amount {
	return fread(buffer, 1, amount, _fd);
}

- (void)close {
	if (_fd) {
		fclose(_fd);
		_fd = NULL;
	}
}

#pragma mark - private

@end
