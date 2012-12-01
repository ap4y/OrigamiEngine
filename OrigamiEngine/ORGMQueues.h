//
// ORGMQueues.h
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

#import <Foundation/Foundation.h>

/**
 The `ORGMQueues` class contains shared singletons instances of the `GCD` queues and sources. It is not recommended to use this singletons outside the library classes. Description provided for clarification.
 */
@interface ORGMQueues : NSObject

/**
 Returns serial queue used as lock for the concurrent operations.
 
 @discussion Serial queue as a tradition lock provides similar functionality as `@synchronized` but it is significantly faster.
 
 @return Singleton of the `GCD` serial queue.
 */
+ (dispatch_queue_t)lock_queue;

/**
 Returns serial queue used for the engine concurrent operations.
 
 @discussion This queue used by ORGMEngine instances to prevent racing conditions between engine actions.
 
 @return Singleton of the `GCD` serial queue.
 */
+ (dispatch_queue_t)processing_queue;

/**
 Returns `GCD` source used to make playback API event-based rather than utilise locks.

 @discussion Due to the nature of processes, it is clear that decoding is significantly faster than playback (converting + output), thus decoder spends a lot of time waiting for the buffer drainage by output unit. It is easy to check via profiler that waiting intervals are bigger that processing intervals for the decoder in most cases, so event-based operations are more efficient.
 
 @return Singleton of the `GCD` source.
 */
+ (dispatch_source_t)buffering_source;
@end
