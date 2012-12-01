//
// ORGMQueues.m
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

#import "ORGMQueues.h"

@implementation ORGMQueues

+ (dispatch_queue_t)lock_queue {
    static dispatch_queue_t _lock_queue;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _lock_queue = dispatch_queue_create("com.origami.lock",
                                            DISPATCH_QUEUE_SERIAL);
    });
    return _lock_queue;
}

+ (dispatch_queue_t)processing_queue {
    static dispatch_queue_t _processing_queue;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _processing_queue = dispatch_queue_create("com.origami.processing",
                                                  DISPATCH_QUEUE_SERIAL);
    });
    return _processing_queue;
}

+ (dispatch_source_t)buffering_source {
    static dispatch_source_t _buffering_source;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _buffering_source = dispatch_source_create(DISPATCH_SOURCE_TYPE_DATA_ADD,
                                                   0, 0, [ORGMQueues processing_queue]);
        dispatch_resume(_buffering_source);
    });
    return _buffering_source;
}

@end
